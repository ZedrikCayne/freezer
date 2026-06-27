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
#include <crankshaft/json.h>
#include <crankshaft/random.h>

#include "freezer.h"

struct UserState {
    struct CS_String *userId;
    struct CS_String *email;
    struct CS_String *freezerId;
    struct CS_String *currentSection;
    bool admin;
    struct CS_String128 _userId;
    struct CS_String128 _email;
    struct CS_String128 _freezerId;
    struct CS_String128 _currentSection;
};

static void setUserStatePointers(struct UserState *userState ) {
    userState->freezerId = (struct CS_String*)&userState->_freezerId;
    userState->currentSection = (struct CS_String*)&userState->_currentSection;
    userState->email = (struct CS_String*)&userState->_email;
    userState->userId = (struct CS_String*)&userState->_userId;
}


static const char *cstringOne = "1";
static struct UserState debugUserState = {0};

struct UserState *initDebugUserState() {
    struct UserState *returnValue = &debugUserState;
    setUserStatePointers( returnValue );
    CS_stringCopyCstringToStatic( returnValue->userId, 128, cstringOne, -1 );
    CS_stringCopyCstringToStatic( returnValue->freezerId, 128, cstringOne, -1 );
    CS_stringCopyCstringToStatic( returnValue->currentSection, 128, cstringOne, -1 );
    CS_stringCopyCstringToStatic( returnValue->email, 128, "test@example.com", -1 );
    return returnValue;
}


struct CS_SqlBackend *freezerBackend = NULL;
static struct CS_SqlSQLITEInitData freezerDbData = { "secrets/freezerDb.mysql" };

static const struct CS_String create_user_to_freezer = CS_STRING("CREATE TABLE IF NOT EXISTS user_to_freezer ( userId TEXT(128), freezerId TEXT(128), admin BOOLEAN, owner BOOLEAN, CONSTRAINT PD_user_to_freezer PRIMARY KEY (userId,freezerId) );");
static const struct CS_String freezer_names = CS_STRING("CREATE TABLE IF NOT EXISTS freezer_names ( freezerId TEXT(128) PRIMARY KEY, freezer_name TEXT(128) )");
static const struct CS_String freezer_sections = CS_STRING("CREATE TABLE IF NOT EXISTS freezer_sections ( freezerId TEXT(128), sectionId TEXT(128), section_name TEXT(128), CONSTRAINT PK_freezer_sections PRIMARY KEY ( freezerId, sectionId ) );");
static const struct CS_String create_freezers = CS_STRING("CREATE TABLE IF NOT EXISTS freezers ( sectionId TEXT(128), upc TEXT(128), num INT, CONSTRAINT PK_freezers PRIMARY KEY (sectionId,upc) );");
static const struct CS_String create_user = CS_STRING("CREATE TABLE IF NOT EXISTS users ( userId TEXT(128) PRIMARY KEY, email TEXT(128), freezerId TEXT(128), sectionId TEXT(128) );");
static const struct CS_String create_items = CS_STRING("CREATE TABLE IF NOT EXISTS items ( upc TEXT(128) PRIMARY KEY, image BOOLEAN, instructions BOOLEAN, nutrition BOOLEAN );");
static const struct CS_String invited = CS_STRING("CREATE TABLE IF NOT EXISTS invites ( email TEXT(128), freezerId TEXT(128), by TEXT(128), accepted BOOLEAN, acknowledged BOOLEAN, CONSTRAINT PK_invites PRIMARY KEY (email,freezerId) )");

static const struct CS_String slash = CS_STRING("/");
static const struct CS_String contentLength = CS_STRING("Content-Length");

static const struct CS_String *imageTypes[] = {
    &CS_STRING("image"),
    &CS_STRING("info"),
    &CS_STRING("nutrition")
};
static const int32_t numImageTypes = CS_ARRAY_SIZE(imageTypes);

static int32_t imageTypeToInt( const char *which ) {
    const struct CS_String *whichStr = CS_stringTempReferenceCstring(which,-1);
    for( int i = 0; i < numImageTypes; ++i ) {
        if( CS_stringStrcmp(imageTypes[i],whichStr) == 0 ) {
            return i;
        }
    }
    return -1;
}

static const struct CS_String *upcFromClientInfo( struct CS_ClientInfo *info ) {
    struct CS_RequestInfo *request = &info->requestInfo;
    const struct CS_String *upc = CS_stringTempStrrstr( &request->uri, &slash );
    if( !upc || upc->length < 1 ) {
        return NULL;
    }
    upc = CS_stringSliceTempReference( upc, 1, -1 );
    if( !CS_stringAlnum(upc) ) {
        return NULL;
    }
    return upc;
}

static const char *freezerItemsByFreezerSQL = "SELECT DISTINCT i.upc,i.image,i.instructions,i.nutrition FROM items AS i INNER JOIN freezers AS f on i.upc = f.upc INNER JOIN freezer_sections AS s ON f.sectionId = s.sectionId WHERE s.freezerId = \"%s\";";
static const char *freezerSectionContentsSQL = "SELECT upc,num FROM freezers WHERE sectionId = \"%s\";";
static const char *productImageName( const struct CS_String *upc, const char *imageType ) {
    return CS_tempBuffSnprintf(1024, "/freezer/products/upc_%s_%s.jpg", CS_stringTempCstring(upc), imageType );
}
static const char *addProductImageSQL = "INSERT INTO items (upc,image,instructions, nutrition) VALUES (\"%s\",TRUE,FALSE,FALSE) ON CONFLICT DO UPDATE SET image = TRUE;";
static bool addProductImage(const struct CS_String *upc) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addProductImageSQL, CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *selectAProductSQL = "SELECT image, instructions, nutrition FROM items WHERE upc = \"%s\";";
static bool productExists( const struct CS_String *upc ) {
    bool returnValue;
    const struct CS_String *sql = CS_stringTempSnprintf(2048, selectAProductSQL, CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    returnValue = (response && response->numRows > 0);
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *selectAProduct( const struct CS_String *upc ) {
    const char *returnValue = NULL;
    const struct CS_String *sql = CS_stringTempSnprintf(2048, selectAProductSQL, CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    if( response == NULL || response->numRows < 1 ) {
        returnValue = NULL;
    } else {
        struct CS_JsonNode *base = CS_jsonNodeNew( 4096 );
        struct CS_JsonNode *root = CS_jsonNodeAppendObject(base,NULL);
        CS_jsonNodeAddUnquotedCstring(root,"upc",CS_stringTempCstring(upc));
        CS_jsonNodeAddUnquotedCstring(root,"image",productImageName(upc,"image"));
        if( response->rows->values[1].intValue )
            CS_jsonNodeAddUnquotedCstring(root,"info",productImageName(upc,"info"));
        else
            CS_jsonNodeAddNull(root,"info");
        if( response->rows->values[2].intValue )
            CS_jsonNodeAddUnquotedCstring(root,"info",productImageName(upc,"nutrition"));
        else
            CS_jsonNodeAddNull(root,"nutrition");
        returnValue = CS_jsonNodePrintableTemp(base);
        CS_jsonFree(base);
    }
    CS_sqlReturnResponse(response);
    
    return returnValue;
}
static const char *addProductInstructionsSQL = "INSERT INTO items (upc,image,instructions, nutrition) VALUES (\"%s\",FALSE,TRUE,FALSE) ON CONFLICT DO UPDATE instructions = TRUE;";
static bool addProductInstructions( const struct CS_String *upc ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addProductInstructionsSQL, CS_stringTempCstring(upc));
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *addProductNutritionSQL = "INSERT INTO items (upc,image,instructions, nutrition) VALUES (\"%s\",FALSE,FALSE,TRUE) ON CONFLICT DO UPDATE SET nutrition = TRUE;";
static bool addProductNutrition( const struct CS_String *upc ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addProductNutritionSQL, CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *addOneToFreezerSectionSQL = "INSERT INTO freezers (sectionId, upc, num) VALUES ( \"%s\", \"%s\", 1 ) ON CONFLICT DO UPDATE SET num = num + 1;";
static bool addOneToFreezerSection( const struct CS_String *sectionId, const struct CS_String *upc ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addOneToFreezerSectionSQL,
            CS_stringTempCstring(sectionId),CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *subOneFromFreezerSectionSQL = "UPDATE freezers SET num = num - 1 WHERE sectionId = \"%s\" AND upc = \"%s\" AND num > 0;";
static bool subOneFromFreezerSection( const struct CS_String *sectionId, const struct CS_String *upc ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, subOneFromFreezerSectionSQL,
            CS_stringTempCstring(sectionId),CS_stringTempCstring(upc) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *addFreezerNameSQL = "INSERT INTO freezer_names (freezerId, freezer_name) VALUES (\"%s\",\"%s\")";
static bool addFreezerName( const char *freezerId, const char *freezer_name ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addFreezerNameSQL,
            freezerId,freezer_name);
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *addFreezerToUserSQL = "INSERT INTO user_to_freezer (userId, freezerId, admin, owner) VALUES (\"%s\", \"%s\", %d, %d);";
static bool addFreezerToUser( const struct CS_String *userId,
                       const struct CS_String *freezerId,
                       bool admin, bool owner ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addFreezerToUserSQL,
            CS_stringTempCstring(userId), CS_stringTempCstring(freezerId),
            admin, owner );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse( response );
    return returnValue;
}
static const char *userAdminOnFreezerSQL = "SELECT admin, owner FROM user_to_freezer WHERE userId = \"%s\" AND freezerId = \"%s\";";
static bool userAdminOnFreezer( const struct CS_String *userId,
                         const struct CS_String *freezerId ) {
    bool returnValue = false;
    const struct CS_String *sql = CS_stringTempSnprintf(2048, userAdminOnFreezerSQL,
            CS_stringTempCstring(userId), CS_stringTempCstring(freezerId) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    if( response && response->numRows == 1 ) {
        returnValue = response->rows->values[0].intValue || response->rows->values[1].intValue;
    }
    if( response ) CS_sqlReturnResponse(response);

    return returnValue;
}
static bool userHasAccessToFreezer( const struct CS_String *userId,
                                    const struct CS_String *freezerId ) {
    bool returnValue = false;
    const struct CS_String *sql = CS_stringTempSnprintf(2048, userAdminOnFreezerSQL,
            CS_stringTempCstring(userId), CS_stringTempCstring(freezerId) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    returnValue = ( response && response->numRows == 1 );
    if( response ) CS_sqlReturnResponse(response);

    return returnValue;
}
static bool hasAdminToFreezer( const struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    return userAdminOnFreezer( userState->userId, userState->freezerId );
}
static bool hasAccessToFreezer( const struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    return userHasAccessToFreezer( userState->userId, userState->freezerId );

}
static const char *findUserSQL = "SELECT email, freezerId, sectionId FROM users WHERE userId = \"%s\";";
static const char *addUserSQL = "INSERT INTO users (userId,email,freezerId,sectionId) VALUES ( \"%s\", \"%s\", \"%s\", \"%s\");";
static bool addUser( const char *userId,
              const char *email,
              const char *freezerId,
              const char *sectionId ) {
    const struct CS_String *sql = CS_stringTempSnprintf(2048, addUserSQL,
            userId, email, freezerId, sectionId );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    bool returnValue = response == NULL;
    CS_sqlReturnResponse(response);
    return returnValue;
}
const char *sectionsForFreezerSQL = "SELECT sectionId, section_name FROM freezer_sections WHERE freezerId = \"%s\";";
const char *sectionsForFreezer( const struct CS_String *freezerId ) {
    const struct CS_String *sql = CS_stringTempSnprintf( 2048, sectionsForFreezerSQL, CS_stringTempCstring(freezerId) );
    const char *returnValue;
    const struct CS_SqlResponse *response = CS_sqlQuery(freezerBackend,sql);
    if( response == NULL ) return NULL;
    struct CS_JsonNode *base = CS_jsonNodeNew( 4096 );
    struct CS_JsonNode *topLevelArray = CS_jsonNodeAppendArray(base,NULL);
    struct CS_SqlRow *currentRow = response->rows;
    for( int32_t i = 0; i < response->numRows; ++i ) {
        struct CS_JsonNode *currentObject = CS_jsonNodeAppendObject(topLevelArray, NULL);
        CS_jsonNodeAppendUnquotedCstring( currentObject, "sectionId", CS_stringTempCstring( currentRow->values[0].stringValue ) );
        CS_jsonNodeAppendUnquotedCstring( currentObject, "section_name", CS_stringTempCstring( currentRow->values[0].stringValue ) );
        currentRow = currentRow->next;
    }
    returnValue = CS_jsonNodePrintableTemp(base);
    CS_jsonFree(base);
    return returnValue;
}
const char *addFreezerSectionSQL = "INSERT INTO freezer_sections ( freezerId, sectionId, section_name ) VALUES ( \"%s\", \"%s\", \"%s\" );";
bool addFreezerSection( const char *freezerId, const char *sectionId, const char *section_name ) {
    bool returnValue = false;
    const struct CS_String *sql = CS_stringTempSnprintf( 2048, addFreezerSectionSQL, freezerId, sectionId, section_name );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    returnValue = response == NULL;
    CS_sqlReturnResponse(response);
    return returnValue;
}
const char *renameFreezerSectionSQL = "UPDATE freezer_sections SET section_name = \"%s\" WHERE  freezerId = \"%s\" AND sectionId = \"%s\";";
bool renameFreezerSection( const struct CS_String *freezerId, const struct CS_String *sectionId, const char *freezerName ) {
    bool returnValue = false;
    const struct CS_String *sql = CS_stringTempSnprintf( 2048, renameFreezerSectionSQL,
            freezerName, CS_stringTempCstring(freezerId),
            CS_stringTempCstring(sectionId) );
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, sql );
    returnValue = response == NULL;
    CS_sqlReturnResponse(response);
    return returnValue;
}


const struct CS_String *creates[] = {
    &create_user_to_freezer,
    &freezer_names,
    &freezer_sections,
    &create_freezers,
    &create_user,
    &create_items,
    &invited
};

struct UserState *CreateUserState(const char *googleId, const char *email) {
    const struct CS_String *query = CS_stringTempSnprintf(2048, findUserSQL, googleId);
    const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, query );
    const char *freezerUUID = NULL;
    const char *sectionUUID = NULL;
    struct UserState *userState = CS_allocZero(sizeof(struct UserState));
    if( userState == NULL ) return NULL;

    setUserStatePointers(userState);

    CS_stringCopyCstringToStatic( userState->userId, 128, googleId, -1 );
    CS_stringCopyCstringToStatic( userState->email, 128, email, -1 );
    if( response == NULL || response->numRows == 0 ) {
        int retries = 6;
        if( response ) CS_sqlReturnResponse(response);
        response = NULL;
        const char *defaultFreezerName;
        defaultFreezerName = CS_tempBuffSnprintf(128,"%s freezer",email);
        do {
            freezerUUID = CS_uuid4CstringTemp();
        } while( --retries>0 && addFreezerName(freezerUUID, defaultFreezerName) );
        if( retries <= 0 ) {
            CS_free(userState);
            return NULL;
        }
        CS_stringCopyCstringToStatic( userState->freezerId, 128, freezerUUID, -1 );
        do {
            sectionUUID = CS_uuid4CstringTemp();
        } while ( --retries>0 && addFreezerSection(freezerUUID, sectionUUID, "Default") );
        if( retries <= 0 ) {
            CS_free(userState);
            return NULL;
        }
        CS_stringCopyCstringToStatic( userState->currentSection, 128, sectionUUID, -1 );
        
        if( addUser(googleId,email,freezerUUID,sectionUUID) ) {
            CS_free(userState);
            return NULL;
        }
        if( addFreezerToUser(userState->userId,userState->freezerId,true,true) ) {
            CS_free(userState);
            return NULL;
        }
    } else {
        CS_stringCopyToStatic( userState->freezerId, response->rows->values[1].stringValue, 128 );
        CS_stringCopyToStatic( userState->currentSection, response->rows->values[2].stringValue, 128 );
        CS_sqlReturnResponse(response);
    }
    return userState;
}

#define SESSION_COOKIE_NAME &CS_STRING("freezer_session")
const struct CS_String googleLoginUri = CS_STRING("/freezer/googlelogin");

static struct CS_HashTable *cheapSessions = NULL;
static struct CS_HashTable *googleIdToSessionId = NULL;

static const char *adminEmail = NULL;

bool startupFreezer( const char *inputAdminEmail ) {
    if( inputAdminEmail == NULL ) return true;
    adminEmail = strdup(inputAdminEmail);
    freezerBackend = CS_sqlInit( &CS_SQL_SQLITE, &freezerDbData );
    if( !freezerBackend ) return true;
    googleIdToSessionId = CS_HASHTABLE_STRING_VOID( 256, CS_HASHTABLE_FLAG_MUTEX|CS_HASHTABLE_FLAG_VERY_PEDANTIC);
    if( !googleIdToSessionId ) return true;
    cheapSessions = CS_HASHTABLE_STRING_VOID( 256, CS_HASHTABLE_FLAG_MUTEX|CS_HASHTABLE_FLAG_VERY_PEDANTIC);
    if( !cheapSessions ) return true;
    CS_srand(time(NULL));

    for( int i = 0; i < CS_ARRAY_SIZE(creates); ++i ) {
        const struct CS_SqlResponse *response = CS_sqlQuery( freezerBackend, creates[i] );
        if( response == NULL ) return true;
        CS_sqlReturnResponse( response );
    }
    
    CS_hashtablePut( cheapSessions, cstringOne, initDebugUserState() );

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
            info->persistentData = (void*)currentSession;
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
        struct UserState *user = CreateUserState( googleId, email->stringValue );
        if( user == NULL )
            return loginPageReturn(info);
        user->admin = isAdmin;
        CS_hashtablePut( googleIdToSessionId, googleId, sessionId );
        CS_hashtablePut( cheapSessions, sessionId, user );
    }
 
    CS_jwtFree( jwt );
    
    return loginAndReturnIndex(info, sessionId);
}

bool uploadImage( struct CS_ClientInfo *info ) {
    const struct CS_String *length = CS_serverGetRequestHeader( info, &contentLength );
    if( !length ) {
        return CS_serverReplyError(info, CS_RESPONSE_411, "Need content length.");
    }

    const struct CS_String *upc = upcFromClientInfo(info);
    if( !upc ) {
        return CS_serverReplyError(info, CS_RESPONSE_400, "Bad product ID." );
    }

    const char *fileName = productImageName( upc, info->appData );
    const char *realFile = CS_tempBuffSnprintf(1024, "root%s", fileName);

    int32_t numBytes = CS_stringAtoi( length );

    FILE *oFile = fopen(realFile,"wb");

    if( !oFile ) {
        return CS_serverReplyError(info, CS_RESPONSE_500, "Unable to write file.");
    }

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

    int32_t imageInt = imageTypeToInt( info->appData );

    switch( imageInt ) {
        case 0:
            addProductImage( upc );
            break;
        case 1:
            addProductInstructions( upc );
            break;
        case 2:
            addProductNutrition( upc );
            break;
        default:
            break;
    }

    const char *selectedProduct = selectAProduct(upc);
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_JSON, selectedProduct, strlen(selectedProduct));
    CS_serverDoReply(info, reply);
    return true;
}

bool deleteImage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool getProduct( struct CS_ClientInfo *info ) {
    const struct CS_String *upc = upcFromClientInfo(info);
    if( upc == NULL ) {
        CS_serverReplyError(info, CS_RESPONSE_400, "Bad UPC code.");
        return true;
    }
    
    const char *selectedProduct = selectAProduct(upc);
    if( selectedProduct == NULL ) {
        CS_serverReplyError(info, CS_RESPONSE_404, "Unknown product.");
        return true;
    }
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_JSON, selectedProduct, strlen(selectedProduct));
    CS_serverDoReply(info, reply);
    return true;
}
bool deleteProduct( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool getMessages( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool deleteMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool acceptMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool sendMessage( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool getFamily( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool inviteFamilyMember( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool removeFamilyMember( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool itemOperationOnFreezerDoesRepliesOnError( struct CS_ClientInfo *info, const struct CS_String *upc ) {
    if( upc == NULL ) {
        CS_serverReplyError(info, CS_RESPONSE_500, "DB not found.");
        return true;
    }
    if( !productExists( upc ) ) {
        CS_serverReplyError(info, CS_RESPONSE_404, "Product not found.");
        return true;
    }
    if( !hasAccessToFreezer(info) ) {
        CS_serverReplyError(info, CS_RESPONSE_401, "Not allowed.");
        return true;
    }
    return false;
}
bool addItem( struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    const struct CS_String *upc = upcFromClientInfo(info);
    if( itemOperationOnFreezerDoesRepliesOnError( info, upc ) ) return true;
    if( addOneToFreezerSection(userState->currentSection, upc) ) {
        CS_serverReplyError(info, CS_RESPONSE_500, "Db Error.");
        return true;
    }
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool removeItem( struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    const struct CS_String *upc = upcFromClientInfo(info);
    if( itemOperationOnFreezerDoesRepliesOnError( info, upc ) ) return true;
    if( subOneFromFreezerSection(userState->currentSection, upc) ) {
        CS_serverReplyError(info, CS_RESPONSE_500, "Database issue.");
        return true;
    }
    struct CS_Reply *reply;
    reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool addSection( struct CS_ClientInfo *info ) {
    //struct UserState *userState = (struct UserState *)info->persistentData;
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool removeSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool switchSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool switchFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool removeFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool addFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool allowAddEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool allowRemoveEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool allowBanEmail( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool listSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    //struct UserState *userState = (struct UserState *)info->persistentData;
    CS_serverDoReply(info, reply);
    return true;
}
bool listFreezer( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    CS_serverDoReply(info, reply);
    return true;
}
bool serveFile( struct CS_ClientInfo *info ) {
    CS_serverFileServer(info);
    return true;
}
bool renameFreezer( struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    if( hasAdminToFreezer(info) ) {
        renameFreezerSection(userState->freezerId,userState->currentSection,NULL);
    } else {
        CS_serverReplyError(info, CS_RESPONSE_401, "Not allowed.");
    }
    return true;
}
bool renameSection( struct CS_ClientInfo *info ) {
    struct CS_Reply *reply = CS_serverCreateReply(info, CS_RESPONSE_200, CS_MIME_DO_NOT_SET, NULL, 0);
    return CS_serverDoReply(info, reply);
}

bool getState( struct CS_ClientInfo *info ) {
    struct UserState *userState = (struct UserState *)info->persistentData;
    struct CS_JsonNode *base = CS_jsonNodeNew(8192);
    struct CS_JsonNode *root = CS_jsonNodeAppendObject(base,NULL);
    struct CS_JsonNode *sections = CS_jsonNodeAddArray(root,"sections");
    struct CS_JsonNode *pantries = CS_jsonNodeAddObject(root,"section_contents");
    const struct CS_String *sql = CS_stringTempSnprintf( 2048, sectionsForFreezerSQL, CS_stringTempCstring(userState->freezerId) );
    const struct CS_SqlResponse *response = CS_sqlQuery(freezerBackend,sql);
    if( response == NULL ) {
        goto DB_ERROR;
    }
    struct CS_SqlRow *currentRow = response->rows;
    while( currentRow ) {
        const char *sectionId = CS_stringTempCstring( currentRow->values[0].stringValue );
        struct CS_JsonNode *currentObject = CS_jsonNodeAddObject(sections, NULL);
        struct CS_JsonNode *currentItems = CS_jsonNodeAddArray(pantries, sectionId );
        CS_jsonNodeAddUnquotedCstring( currentObject, "sectionId", CS_stringTempCstring( currentRow->values[0].stringValue ) );
        CS_jsonNodeAddUnquotedCstring( currentObject, "section_name", CS_stringTempCstring( currentRow->values[0].stringValue ) );
        CS_jsonNodeAddBool( currentObject, "current", CS_stringStrcmp( currentRow->values[0].stringValue, userState->currentSection) == 0 );
        sql = CS_stringTempSnprintf(2048, freezerSectionContentsSQL,sectionId ); 
        const struct CS_SqlResponse *sectionResponse = CS_sqlQuery(freezerBackend,sql);
        if( sectionResponse == NULL ) {
            goto DB_ERROR;
        }
        const struct CS_SqlRow *itemRow = sectionResponse->rows;
        while( itemRow ) {
            struct CS_JsonNode *item = CS_jsonNodeAddObject(currentItems,NULL);
            CS_jsonNodeAddUnquotedCstring( item, "upc", CS_stringTempCstring( itemRow->values[0].stringValue ) );
            CS_jsonNodeAddInteger( item, "num", itemRow->values[1].intValue );
            itemRow = itemRow->next;
        }
        currentRow = currentRow->next;
        CS_sqlReturnResponse( sectionResponse );
    }
    CS_sqlReturnResponse(response);
    sql = CS_stringTempSnprintf(2048, freezerItemsByFreezerSQL, CS_stringTempCstring(userState->freezerId));
    response = CS_sqlQuery( freezerBackend, sql );
    if( response == NULL ) {
        goto DB_ERROR;
    }
    struct CS_JsonNode *items = CS_jsonNodeAddObject(root,"itemdb");
    struct CS_SqlRow *itemRow = response->rows;
    while( itemRow ) {
        const struct CS_String *upc = itemRow->values[0].stringValue;
        struct CS_JsonNode *itemNode = CS_jsonNodeAddObject(items, CS_stringTempCstring(upc));
        if( itemRow->values[1].intValue )
            CS_jsonNodeAddUnquotedCstring(itemNode,"image",productImageName(upc,"image"));
        else
            CS_jsonNodeAddNull(root,"image");
        if( itemRow->values[2].intValue )
            CS_jsonNodeAddUnquotedCstring(itemNode,"info",productImageName(upc,"info"));
        else
            CS_jsonNodeAddNull(root,"info");
        if( itemRow->values[3].intValue )
            CS_jsonNodeAddUnquotedCstring(itemNode,"nutrition",productImageName(upc,"nutrition"));
        else
            CS_jsonNodeAddNull(root,"nutrition");
        itemRow = itemRow->next;
    }
    CS_sqlReturnResponse(response);
    const char *jsonReply = CS_jsonNodePrintableTemp(base);
    struct CS_Reply *reply = CS_serverCreateReply( info, CS_RESPONSE_200, CS_MIME_JSON, jsonReply, strlen(jsonReply) );
    CS_serverDoReply(info, reply);
    return true;

DB_ERROR:
    CS_jsonFree(base);
    CS_serverReplyError(info, CS_RESPONSE_500, "Database issue");
    return true;

}
bool logfilter( struct CS_ClientInfo *info ) {
    CS_LOG_LOUD("REQUEST: %.*s", info->requestInfo.uri.length, info->requestInfo.uri.data);
return false;
}
