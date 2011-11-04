#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(vpnclient)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

# need libtnc
find_package(Libtnc REQUIRED)
include_directories(${LIBTNC_INCLUDE_DIR})
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

# use static libraries of libtnc
set(LIBTNC_USE_STATIC "TRUE")

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/vpnclient/main.cpp)
set(src ${src} src/vpnclient/TNCClient.cpp)
set(src ${src} src/vpnclient/TNCMockClient.cpp)
set(src ${src} src/vpnclient/LibTNCClient.cpp)

# define the example executable
add_executable(${PROJECT_NAME} ${src})

# add libraries
target_link_libraries(${PROJECT_NAME} -lssl ${Boost_LIBRARIES} tncvpn tncutil
	${LIBTNC_LIBRARIES} ${LIBXML2_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CONFIG_DIR  "${SYSCONF_INSTALL_DIR}/tnc/ifttls/client")
set(CONFIG_FILE  "${CONFIG_DIR}/client.conf" )

IF(NOT EXISTS ${CONFIG_FILE})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/client/client.conf
        DESTINATION ${CONFIG_DIR})
ENDIF()

