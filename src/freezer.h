#include <crankshaft/server.h>

#ifdef __cplusplus
extern "C" {
#endif

bool startupFreezer( const char *adminEmail );
bool stopFreezer();

bool cookieFilter( struct CS_ClientInfo *info );
bool logBody( struct CS_ClientInfo *info );
bool googleLogin( struct CS_ClientInfo *info );
bool uploadImage( struct CS_ClientInfo *info );
bool getProduct( struct CS_ClientInfo *info );
bool getPantry( struct CS_ClientInfo *info );
bool addItem( struct CS_ClientInfo *info );
bool deletePantrySection( struct CS_ClientInfo *info );
bool createPantrySection( struct CS_ClientInfo *info );
bool deleteImage( struct CS_ClientInfo *info );
bool getProduct( struct CS_ClientInfo *info );
bool deleteProduct( struct CS_ClientInfo *info );
bool getMessages( struct CS_ClientInfo *info );
bool deleteMessage( struct CS_ClientInfo *info );
bool acceptMessage( struct CS_ClientInfo *info );
bool sendMessage( struct CS_ClientInfo *info );
bool getFamily( struct CS_ClientInfo *info );
bool inviteFamilyMember( struct CS_ClientInfo *info );
bool removeFamilyMember( struct CS_ClientInfo *info );
bool addItem( struct CS_ClientInfo *info );
bool removeItem( struct CS_ClientInfo *info );
bool addSection( struct CS_ClientInfo *info );
bool removeSection( struct CS_ClientInfo *info );
bool switchSection( struct CS_ClientInfo *info );
bool switchFreezer( struct CS_ClientInfo *info );
bool removeFreezer( struct CS_ClientInfo *info );
bool addFreezer( struct CS_ClientInfo *info );
bool allowAddEmail( struct CS_ClientInfo *info );
bool allowRemoveEmail( struct CS_ClientInfo *info );
bool allowBanEmail( struct CS_ClientInfo *info );

#ifdef __cplusplus
}
#endif
bool CS_serverFileServer( struct CS_ClientInfo *info );
bool CS_serverFileServer( struct CS_ClientInfo *info );
bool listSection( struct CS_ClientInfo *info );
bool listFreezer( struct CS_ClientInfo *info );
bool CS_serverFileServer( struct CS_ClientInfo *info );
bool serveFile( struct CS_ClientInfo *info );
bool renameFreezer( struct CS_ClientInfo *info );
bool renameSection( struct CS_ClientInfo *info );
