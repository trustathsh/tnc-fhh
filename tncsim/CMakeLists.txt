#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(tncsim)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

#set tncsim_config file
set(TNCSIM_CONFIG_FILE  "tncsim_config" CACHE PATH "The tncsim configuration file.")
#set tncsim config dir
set(TNCSIM_CONFIG_DIR  "${SYSCONF_INSTALL_DIR}/tnc" CACHE PATH "The directory where to copy the tncsim configuration file.")
#set tncsim config
set(TNCSIM_CONFIG "${TNCSIM_CONFIG_DIR}/${TNCSIM_CONFIG_FILE}" CACHE PATH "The tncsim configuration.")

#set server and client to default values
set(SERVER "LIBTNC" CACHE STRING "Please set SERVER to \"TNCFHH\" or \"LIBTNC\"")
set(CLIENT "LIBTNC" CACHE STRING "Please set CLIENT to \"NETWORK\" or \"LIBTNC\"")

# display status message for important variables
MESSAGE( STATUS "SERVER = ${SERVER}" )
MESSAGE( STATUS "CLIENT = ${CLIENT}" )
MESSAGE( STATUS "TNCS_LIBRARY = ${TNCS_LIBRARY}" )
MESSAGE( STATUS "NET_INTERFACE = ${NET_INTERFACE}" )

# use static libraries of libtnc
set(LIBTNC_USE_STATIC "TRUE")

# check and set specially server and client configure
if (SERVER STREQUAL "TNCFHH")
	message(STATUS "Using TNC@FHH Server")
    set(src ${src} src/server/tncfhh/TncfhhTNCS.cpp)
    set(OPT_TNCS_LIBRARIES tncs)
elseif (SERVER STREQUAL "LIBTNC")
	message(STATUS "Using libtnc Server")
    find_package(LibXml2 REQUIRED)
	find_package(Libtnc REQUIRED)
	include_directories(${LIBTNC_INCLUDE_DIR})
    include_directories(${LIBXML2_INCLUDE_DIR})
    set(src ${src} src/server/libtnc/LibtncTNCS.cpp)
else ()
	message(SEND_ERROR "Please set SERVER to \"TNCFHH\" or \"LIBTNC\"")
endif (SERVER STREQUAL "TNCFHH")

if (CLIENT STREQUAL "NETWORK")
	message(SEND_ERROR "Using network Client")
elseif (CLIENT STREQUAL "LIBTNC")
	message(STATUS "Using libtnc Client")
    find_package(LibXml2 REQUIRED)
	find_package(Libtnc REQUIRED)
	include_directories(${LIBTNC_INCLUDE_DIR})
    include_directories(${LIBXML2_INCLUDE_DIR})
    set(src ${src} src/client/libtnc/LibtncTNCC.cpp)
else ()
	message(SEND_ERROR "Please set CLIENT to \"NETWORK\" or \"LIBTNC\"")
endif (CLIENT STREQUAL "NETWORK")

# configuration header
configure_file(src/tncsimConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/tncsimConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory(src)

# set sourcecode
# fast detected with:
#     $ find src/ -iname "*.cpp" -exec echo "set(src \${src} {})" \;
set(src ${src} src/client/AbstractTNCC.cpp)
set(src ${src} src/server/AbstractTNCS.cpp)
set(src ${src} src/server/Finished.cpp)
set(src ${src} src/TNCCSData.cpp)
set(src ${src} src/tncsim.cpp)

# define executable
add_executable(tncsim ${src})
# or define shared object
#add_library(tncsim SHARED ${src})

# add libraries

target_link_libraries(tncsim ${LIBXML2_LIBRARIES} ${LIBTNC_LIBRARIES} ${OPT_TNCS_LIBRARIES})

# add install target
install(TARGETS tncsim RUNTIME DESTINATION bin)         

# TODO add variables
IF(NOT EXISTS ${TNCSIM_CONFIG})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/tncsim_config DESTINATION ${SYSCONF_INSTALL_DIR}/tnc)
ENDIF()

