#include <crankshaft/server.h>

#ifdef __cplusplus
extern "C" {
#endif

bool startupFreezer( const char *adminEmail );
bool stopFreezer();

bool cookieFilter( struct CS_ClientInfo *info );
extern struct CS_String googleLoginUri;
bool googleLogin( struct CS_ClientInfo *info );
bool uploadImage( struct CS_ClientInfo *info );
bool getProduct( struct CS_ClientInfo *info );
bool getPantry( struct CS_ClientInfo *info );
bool addItem( struct CS_ClientInfo *info );
bool deletePantrySection( struct CS_ClientInfo *info );
bool createPantrySection( struct CS_ClientInfo *info );


#ifdef __cplusplus
}
#endif
