#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(tncvpn)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

include_directories("src/tncvpn")

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/tncvpn/libtnac/AllowDenyPacket.cpp)
set(src ${src} src/tncvpn/ifttls/Envelope.cpp)
set(src ${src} src/tncvpn/ifttls/IFTTLSConn.cpp)
set(src ${src} src/tncvpn/ifttls/exceptions/ReceiveException.cpp)
set(src ${src} src/tncvpn/ifttls/exceptions/VersionException.cpp)
set(src ${src} src/tncvpn/iel/Endpoint.cpp)

# define shared object
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries
target_link_libraries(${PROJECT_NAME} ${Log4cxx_LIBRARY}
	${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME}${TYPE} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install (for development)
install(DIRECTORY src/ DESTINATION include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.svn*" EXCLUDE)
                  
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
