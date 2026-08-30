#include <crankshaft/server.h>

#ifdef __cplusplus
extern "C" {
#endif

bool startupFreezer( const char *adminEmail );
bool stopFreezer();

bool cookieFilter( struct CS_RequestInfo *info );
bool logBody( struct CS_RequestInfo *info );
bool googleLogin( struct CS_RequestInfo *info );
bool uploadImage( struct CS_RequestInfo *info );
bool getProduct( struct CS_RequestInfo *info );
bool getPantry( struct CS_RequestInfo *info );
bool addItem( struct CS_RequestInfo *info );
bool deletePantrySection( struct CS_RequestInfo *info );
bool createPantrySection( struct CS_RequestInfo *info );
bool deleteImage( struct CS_RequestInfo *info );
bool getProduct( struct CS_RequestInfo *info );
bool deleteProduct( struct CS_RequestInfo *info );
bool getMessages( struct CS_RequestInfo *info );
bool deleteMessage( struct CS_RequestInfo *info );
bool acceptMessage( struct CS_RequestInfo *info );
bool sendMessage( struct CS_RequestInfo *info );
bool getFamily( struct CS_RequestInfo *info );
bool inviteFamilyMember( struct CS_RequestInfo *info );
bool removeFamilyMember( struct CS_RequestInfo *info );
bool addItem( struct CS_RequestInfo *info );
bool removeItem( struct CS_RequestInfo *info );
bool addSection( struct CS_RequestInfo *info );
bool removeSection( struct CS_RequestInfo *info );
bool switchSection( struct CS_RequestInfo *info );
bool switchFreezer( struct CS_RequestInfo *info );
bool removeFreezer( struct CS_RequestInfo *info );
bool addFreezer( struct CS_RequestInfo *info );
bool allowAddEmail( struct CS_RequestInfo *info );
bool allowRemoveEmail( struct CS_RequestInfo *info );
bool allowBanEmail( struct CS_RequestInfo *info );
bool listSection( struct CS_RequestInfo *info );
bool listFreezer( struct CS_RequestInfo *info );
bool serveFile( struct CS_RequestInfo *info );
bool renameFreezer( struct CS_RequestInfo *info );
bool renameSection( struct CS_RequestInfo *info );
bool getState( struct CS_RequestInfo *info );
#ifdef __cplusplus
}
#endif
bool logfilter( struct CS_RequestInfo *info );
