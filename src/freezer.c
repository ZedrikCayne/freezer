#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <sys/socket.h>

#include <crankshaft/alloc.h>
#include <crankshaft/uuid.h>
#include <crankshaft/logger.h>
#include <crankshaft/commandline.h>
#include <crankshaft/server.h>
#include <crankshaft/tempbuff.h>
#include <crankshaft/html.h>
#include <crankshaft/googleservices.h>
#include <crankshaft/mime.h>
#include <crankshaft/jwt.h>
#include <crankshaft/json.h>
#include <crankshaft/storage.h>
#include <crankshaft/jwtkeychain.h>
#include <crankshaft/ssl.h>
#include <crankshaft/util.h>
#include <crankshaft/string.h>
#include <crankshaft/jwt.h>
#include <crankshaft/hashtable.h>
#include <crankshaft/storage.h>
#include <crankshaft/sql.h>
#include <crankshaft/stringbuilder.h>

#include "freezer.h"

const struct CS_String create_user_to_freezer = CS_STRING("CREATE TABLE IF NOT EXISTS user_to_freezer ( userId TEXT(128) PRIMARY KEY, freezerId TEXT(128), admin BOOLEAN, owner BOOLEAN );");
const struct CS_String freezer_names = CS_STRING("CREATE TABLE IF NOT EXISTS freezer_names ( freezerId TEXT(128) PRIMARY KEY, freezer_name TEXT(128) )");
const struct CS_String create_freezers = CS_STRING("CREATE TABLE IF NOT EXISTS freezers ( freezerId TEXT(128), section TEXT(128), upc TEXT(128), num INT, CONSTRAINT PK_freezers PRIMARY KEY (freezerId,section,upc) );");
const struct CS_String create_user = CS_STRING("CREATE TABLE IF NOT EXISTS users ( userId TEXT(128) PRIMARY KEY, email TEXT(128) );");
const struct CS_String create_items = CS_STRING("CREATE TABLE IF NOT EXISTS items ( upc TEXT(128) PRIMARY KEY, image BOOLEAN, instructions BOOLEAN, nutrition BOOLEAN );");
const struct CS_String invited = CS_STRING("CREATE TABLE IF NOT EXISTS invites ( email TEXT(128), freezerId TEXT(128), by TEXT(128), accepted BOOLEAN, acknowledged BOOLEAN, CONSTRAINT PK_invites PRIMARY KEY (email,freezerId) )");

const struct CS_String *creates[] = {
    &create_user_to_freezer,
    &freezer_names,
    &create_freezers,
    &create_user,
    &create_items,
    &invited
};

static const struct CS_String slash = CS_STRING("/");
static const struct CS_String invalid_chars = CS_STRING(". &;?#");
static const struct CS_String contentLength = CS_STRING("Content-Length");

struct UserState {
    struct CS_String128 freezerId;
    struct CS_String128 userId;
    bool admin;
};

struct UserState *CreateUserState(const char *googleId) {
    struct UserState *userState = CS_allocZero(sizeof(struct UserState));
    CS_stringCopyCstringToStatic( (struct CS_String*)&userState->userId, 128, googleId, -1 );
    return userState;
}

#define SESSION_COOKIE_NAME &CS_STRING("freezer_session")
const struct CS_String googleLoginUri = CS_STRING("/freezer/googlelogin");

static const struct CS_Storage *longTermStorage = NULL;
static struct CS_HashTable *cheapSessions = NULL;
static struct CS_HashTable *googleIdToSessionId = NULL;

static const char *sessionThatIsAdmin = NULL;
static const char *adminEmail = NULL;

struct CS_SqlBackend *freezerBackend = NULL;
struct CS_SqlSQLITEInitData freezerDbData = { "secrets/freezerDb.mysql" };

bool startupFreezer( const char *inputAdminEmail ) {
    if( inputAdminEmail == NULL ) return true;
    adminEmail = strdup(inputAdminEmail);
    freezerBackend = CS_sqlInit( &CS_SQL_SQLITE, &freezerDbData );
    if( !freezerBackend ) return true;
    googleIdToSessionId = CS_HASHTABLE_STRING_VOID( 256, CS_HASHTABLE_FLAG_MUTEX|CS_HASHTABLE_FLAG_VERY_PEDANTIC);
    if( !googleIdToSessionId ) return true;
    cheapSessions = CS_HASHTABLE_STRING_VOID( 256, CS_HASHTABLE_FLAG_MUTEX|CS_HASHTABLE_FLAG_VERY_PEDANTIC);
    if( !cheapSessions ) return true;

    for( int i = 0; i < CS_ARRAY_SIZE(creates); ++i ) {
        const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, creates[i] );
        if( response == NULL ) return true;
        CS_sqlReturnResponse( response );
    }
    
    return false;
}

bool stopFreezer() {
    if( freezerBackend ) CS_sqlClose(freezerBackend);
    freezerBackend = NULL;
    if( googleIdToSessionId ) CS_hashtableFree( googleIdToSessionId );
    googleIdToSessionId = NULL;
    if( cheapSessions ) CS_hashtableFree( cheapSessions );
    cheapSessions = NULL;

    return false;
}

bool loginAndReturnIndex( struct CS_ClientInfo *info, const char *sessionCookie ) {
    struct CS_Reply *reply = CS_serverCreateReply( info, CS_RESPONSE_302, CS_MIME_HTML, NULL, 0 );
    CS_serverSetReplyCookie( reply, SESSION_COOKIE_NAME, CS_stringTempReferenceCstring(sessionCookie,-1), true, CS_REPLY_COOKIE_SAMESITE_LAX );
    CS_serverSetReplyHeader( reply, &CS_STRING("Location"), &CS_STRING("/freezer") );
    CS_serverDoReply( info, reply );
    return true;
}

static const struct CS_String localhost = CS_STRING("localhost");
static const struct CS_String aHost = CS_STRING("webmud.publicvm.com");
bool loginPageReturn( struct CS_ClientInfo *info ) {
    struct CS_HtmlNode *root = CS_htmlCreateRoot("html",2048);
    struct CS_HtmlNode *head = CS_htmlAddContainerAfter( root, "head" );
    struct CS_HtmlNode *meta = CS_htmlAddContainerAfter( head, "meta" );
    CS_htmlAddAttribute( meta, "charset", "utf-8" );
    struct CS_HtmlNode *title = CS_htmlAddContainerAfter( head, "title" );
    CS_htmlSetContents( title, CS_tempBuffSnprintf(1024, "Freezer login page"), false );
    struct CS_HtmlNode *body = CS_htmlAddContainerAfter( root, "body" );
    struct CS_HtmlNode *script = CS_htmlAddContainerAfter(body, "script");
    CS_htmlAddAttribute( script, "async", NULL );
    CS_htmlAddAttribute( script, "src", "https://accounts.google.com/gsi/client" );
    const struct CS_String host_string = CS_STRING("Host");
    const struct CS_String *host = CS_serverGetRequestHeader(info,&host_string);
    if( host == NULL ) host = &localhost;

    struct CS_HtmlNode *div = CS_htmlAddContainerAfter( body, "div" );
    CS_htmlAddAttribute( div, "id", "g_id_onload" );
    CS_htmlAddAttribute( div, "data-client_id", CS_GS_getClientID() );
    CS_htmlAddAttribute( div, "data-login_uri",
           CS_tempBuffSnprintf( 1024, "https://%s%s", 
               CS_stringTempCstring( host ),
               CS_stringTempCstring( &googleLoginUri ) ) );

    div = CS_htmlAddContainerAfter( body, "div" );
    CS_htmlAddAttribute( div, "class", "g_id_signin" );
    CS_htmlAddAttribute( div, "data-auto_prompt", "false" );
    CS_htmlAddAttribute( div, "data-type", "standard" );
    CS_htmlAddAttribute( div, "data-size", "large" );
    CS_htmlAddAttribute( div, "data-theme", "outline" );
    CS_htmlAddAttribute( div, "data-text", "sign_in_with" );
    CS_htmlAddAttribute( div, "data-shape", "rectangular" );
    CS_htmlAddAttribute( div, "data-logo_alignment", "left" );

    div = CS_htmlAddContainerAfter( body, "div" );
    CS_htmlSetContents( div, "What's in your freezer? Log in to find out!", false );

    struct CS_StringBuilder *sb = CS_htmlToStringBuilder( root, 2048, false );
    struct CS_Reply *reply = CS_serverCreateReply( info, CS_RESPONSE_200, CS_MIME_HTML, CS_SB_buffer( sb ), CS_SB_size( sb ) );
    CS_serverDoReply( info, reply );
    CS_SB_free( sb );
    CS_htmlFree( root );
    return true;
}

const struct CS_String logBodyUri = CS_STRING("/freezer/api/logbody");
bool logBody( struct CS_ClientInfo *info ) {
    struct CS_StringBuilder *sb = CS_SB_create( 8192 );
    const struct CS_String *length = CS_serverGetRequestHeader( info, &contentLength );
    if( !length ) {
        return CS_serverReplyError(info, CS_RESPONSE_411, "Need content length.");
    }
    int32_t numBytes = CS_stringAtoi( length );
    int32_t numBytesWritten = 0;

    if( CS_PP_dataSize( info->buffer ) > 0 ) {
        numBytesWritten += CS_PP_dataSize( info->buffer );
        CS_SB_appendBytes( sb, CS_PP_startOfData( info->buffer ), CS_PP_dataSize( info->buffer ) );
        CS_PP_write( info->buffer, CS_PP_dataSize(info->buffer) );
    }

    while( numBytesWritten < numBytes ) {
        if( CS_serverFillIncomingBuffer( info ) <= 0 ) {
            return CS_serverReplyError(info, CS_RESPONSE_500, "Incoming data too small");
        }
        CS_SB_appendBytes( sb, CS_PP_startOfData( info->buffer ), CS_PP_dataSize( info->buffer ) );
        numBytesWritten += CS_PP_write( info->buffer, CS_PP_dataSize(info->buffer) );
    }
    CS_LOG_LOUD("LOG BODY: %s",sb->buffer);
    CS_SB_free( sb );
    struct CS_Reply *reply = CS_serverCreateReply( info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0 );
    return CS_serverDoReply( info, reply );
}

bool cookieFilter( struct CS_ClientInfo *info ) {
    const struct CS_String *cookieValue = CS_serverGetRequestCookie( info, SESSION_COOKIE_NAME );
    if( cookieValue != NULL ) {
        const void *currentSession = CS_hashtableGet( cheapSessions, CS_stringTempCstring(cookieValue) );
        if( currentSession != CS_HASHTABLE_ERROR && currentSession != NULL ) {
            return false;
        }
    }
    return loginPageReturn(info);
}

bool googleLogin( struct CS_ClientInfo *info ) {
    const struct CS_String g_csrf_token_string = CS_STRING("g_csrf_token");
    const struct CS_String *g_csrf_header = CS_serverGetRequestCookie(info, &g_csrf_token_string);
    if( !g_csrf_header ) {
        CS_LOG_ERROR( "Login missing csrf token header." );
        return loginPageReturn(info);
    }
    const struct CS_String *g_csrf_form = CS_serverGetRequestFormParameter(info, &g_csrf_token_string );
    if( !g_csrf_form ) {
        CS_LOG_ERROR( "Login missing csrf token form." );
        return loginPageReturn(info);
    }
    if( CS_stringStrcmp( g_csrf_header, g_csrf_form ) != 0 ) {
        CS_LOG_ERROR( "Login csrf different from form csrf." );
        return loginPageReturn(info);
    }
    const struct CS_String credential_string = CS_STRING("credential");
    const struct CS_String *credential = CS_serverGetRequestFormParameter(info, &credential_string);
    if( !credential ) {
        CS_LOG_ERROR( "Login missing credential." );
        return loginPageReturn(info);
    }
    const struct CS_Jwt *jwt = CS_jwtParse( credential->data, credential->length, 1024 );
    if( !jwt ) {
        CS_LOG_ERROR( "Failed to parse a jwt out of the credential." );
        return loginPageReturn(info);
    }
    if( !CS_GS_jwtVerify(jwt) ) {
        CS_LOG_ERROR( "Failed to verify a jwt." );
        return loginPageReturn(info);
    }

    struct CS_JsonNode *subject = CS_jsonNodeByPath( jwt->jsonPayload, "sub" );
    if( !subject ) {
        CS_jwtFree(jwt);
        return loginPageReturn(info);
    }
    char *googleId = strndup( (char*)CS_jsonNodeValueAsTempCstring( subject ), 64 );

    bool isAdmin = false;
    struct CS_JsonNode *email = CS_jsonNodeByPath( jwt->jsonPayload, "email" );
    if( email && strcmp( email->stringValue, adminEmail ) == 0 ) {
        isAdmin = true;
    }

    const void *sessionId = CS_hashtableGet( googleIdToSessionId, googleId );

    if( sessionId == CS_HASHTABLE_ERROR ) {
        sessionId = CS_uuid4CstringTemp();
        struct UserState *user = CreateUserState( googleId );
        user->admin = isAdmin;
        const struct CS_String *queryForFreezer = CS_stringTempSnprintf( 2048, "SELECT freezerId FROM user_to_freezer WHERE userid = \"%s\";", googleId );
        const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, queryForFreezer );

        if( !response || response->numRows == 0 ) {
            //Create a new freezer.
            CS_sqlReturnResponse(response);
            const char *freezerId = CS_uuid4CstringTemp();
            const struct CS_String *queryToAdd = CS_stringTempSnprintf( 2048, "INSERT INTO user_to_freezer (userId, freezerId) VALUES (\"%s\",\"%s\");", googleId, freezerId );
            response = CS_sqlQuery( freezerBackend, queryToAdd );
            if( response == NULL ) {
                return loginPageReturn(info);
            }
            CS_stringCopyCstringToStatic( (struct CS_String*)&user->freezerId, 128, freezerId, -1 );
            CS_sqlReturnResponse(response);
        } else {
            CS_stringCopyToStatic( (struct CS_String*)&user->freezerId, response->rows->values->stringValue, 128 );
            CS_sqlReturnResponse(response);
        }
        CS_hashtablePut( googleIdToSessionId, googleId, sessionId );
        CS_hashtablePut( cheapSessions, sessionId, user );
    }
 
    CS_jwtFree( jwt );
    
    return loginAndReturnIndex(info, sessionId);
}

bool killFreezer() {
    return false;
}

bool uploadImage( struct CS_ClientInfo *info ) {
    struct CS_RequestInfo *request = &info->requestInfo;
    const struct CS_String *parsedFile = CS_stringTempStrrstr( &request->uri, &slash );
    if( !parsedFile || parsedFile->length < 5 ) {
        return CS_serverReplyError(info, CS_RESPONSE_400, "Bad product ID." );
    }
    parsedFile = CS_stringSliceTempReference( parsedFile, 1, -1 );
    const char *savePtr = NULL;
    const struct CS_String *maybeToken = CS_stringTempStrtok( parsedFile, &invalid_chars, &savePtr );
    if( maybeToken && CS_stringStrcmp(parsedFile,maybeToken) )  {
        return CS_serverReplyError(info, CS_RESPONSE_400, "Invalid characters in product id." );
    }

    const char *fileName = CS_tempBuffSnprintf(1024, "/freezer/products/upc_%s_%s.jpg", CS_stringTempCstring(parsedFile), (char*)info->appData );
    const char *realFile = CS_tempBuffSnprintf(1024, "root%s", fileName);

    const struct CS_String *length = CS_serverGetRequestHeader( info, &contentLength );
    if( !length ) {
        return CS_serverReplyError(info, CS_RESPONSE_411, "Need content length.");
    }

    FILE *oFile = fopen(realFile,"wb");

    if( !oFile ) {
        return CS_serverReplyError(info, CS_RESPONSE_500, "Unable to write file.");
    }

    int32_t numBytes = CS_stringAtoi( length );
    int32_t numBytesWritten = 0;

    if( CS_PP_dataSize( info->buffer ) > 0 ) {
        numBytesWritten += CS_PP_writeToFILE( info->buffer, oFile );
    }

    while( numBytesWritten < numBytes ) {
        if( CS_serverFillIncomingBuffer( info ) <= 0 ) {
            return CS_serverReplyError(info, CS_RESPONSE_500, "Incoming data too small");
        }
        numBytesWritten += CS_PP_writeToFILE( info->buffer, oFile );
    }
    fclose( oFile );

    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool deleteImage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool getProduct( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool deleteProduct( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool getMessages( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool deleteMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool acceptMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool sendMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool getFamily( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool inviteFamilyMember( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool removeFamilyMember( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool addItem( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool removeItem( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool addSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool removeSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool switchSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool switchFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool removeFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool addFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool allowAddEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool allowRemoveEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
bool allowBanEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}
