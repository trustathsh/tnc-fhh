#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(vpnpep)
MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

#set tnc config dir
set(PEP_CONFIG_DIR  "${SYSCONF_INSTALL_DIR}/tnc/ifttls/pep" CACHE PATH "Where to copy PEP stuff.")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/vpnpep/EndpointDB.cpp)
set(src ${src} src/vpnpep/main.cpp)

# define the example executable
add_executable(pep ${src})

# add libraries
target_link_libraries(pep  ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} tncvpn tncutil)

# define install targets
install(TARGETS pep
        DESTINATION bin)
        
IF(UNIX)
    # define install (for development)
	install(DIRECTORY conf/ 
        	DESTINATION "${PEP_CONFIG_DIR}"
        	FILES_MATCHING
        	PATTERN "*.pem"
        	PATTERN "*.properties"
        	PATTERN "*.conf"
        	PATTERN "*.sh"
        	PATTERN "*.svn*" EXCLUDE)
ELSE(UNIX)
    INSTALL(CODE "MESSAGE(\"INFO: YOU MUST CONFIGURE TNC-SERVER MANUALLY!\")")
ENDIF(UNIX)
