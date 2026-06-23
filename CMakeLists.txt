cmake_minimum_required(VERSION 3.24)

project( freezer )

if ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
set(MACOSX TRUE)
find_package(OpenSSL)
else()
endif()

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set(CMAKE_BUILD_TYPE Debug)

file( GLOB SOURCES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.c )

add_executable( freezer ${SOURCES} )

target_include_directories( freezer PUBLIC crankshaft/include )

target_link_directories( freezer PUBLIC crankshaft/lib )

target_compile_options( freezer PUBLIC -Wall )

if( MACOSX )
if( OPENSSL_FOUND )
target_include_directories( freezer PUBLIC ${OPENSSL_INCLUDE_DIR} )
target_link_libraries( freezer ${OPENSSL_LIBRARIES} )
target_link_libraries( freezer -lcrankshaft )
endif()
else()
target_link_libraries( freezer m ssl crypto pthread m z dl crankshaft )
endif()
