#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(naavpn)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/naavpn/TNCServer.cpp)
set(src ${src} src/naavpn/MessageHandler.cpp)
set(src ${src} src/naavpn/PEPClient.cpp)
set(src ${src} src/naavpn/PEPNotifier.cpp)
set(src ${src} src/naavpn/TNCSession.cpp)

# define shared object
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries
target_link_libraries(${PROJECT_NAME} ${Log4cxx_LIBRARY}
	${Boost_LIBRARIES} tncs)

# define the example executable
add_executable(pdp src/server/main.cpp ${src})

target_link_libraries(pdp ${Log4cxx_LIBRARY} ${Boost_LIBRARIES} tncs
	tncvpn tncutil)

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install targets
install(DIRECTORY src/naavpn
		DESTINATION include FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.svn*" EXCLUDE)

SET(CONFIGFILES ca.pem dh512.pem log4j.properties pdp.conf server.pem tnc_vpn.conf)
FOREACH(CFILE ${CONFIGFILES})
	IF(NOT EXISTS ${SYSCONF_INSTALL_DIR}/tnc/ifttls/pdp/${CFILE})
		install(FILES
			${CMAKE_CURRENT_SOURCE_DIR}/conf/${CFILE}
			DESTINATION ${SYSCONF_INSTALL_DIR}/tnc/ifttls/pdp)
	ENDIF(NOT EXISTS ${SYSCONF_INSTALL_DIR}/tnc/ifttls/pdp/${CFILE})
ENDFOREACH(CFILE ${CONFIGFILES})


install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS pdp RUNTIME DESTINATION bin)
