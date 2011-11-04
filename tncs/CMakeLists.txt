#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(tncs)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

#set tnc_config file
set(TNCS_CONFIG_FILE  "tnc_config" CACHE PATH "The TNCS config file (lists IMVs to be loaded).")
set(XACML_IMF_IMV_CONFIG_FILE  "xacml-ifm-imv.conf" CACHE PATH "The XACML config file; maps IMVs to IF-M component types.")

#set tnc config dir
set(TNCS_CONFIG_DIR  "${SYSCONF_INSTALL_DIR}" CACHE PATH "The directory where to copy the TNCS config file.")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncutil/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncxacml/src/)

# find additionally required libraries 
find_package(LTDL REQUIRED)
include_directories(${LTDL_INCLUDE_DIR})

find_package(XercesC 2.8 REQUIRED)
include_directories(${XercesC_INCLUDE_DIR})

# configuration header
configure_file(src/tncs/tncsConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/tncsConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/tncs/iel/TNCCSData.cpp)
set(src ${src} src/tncs/iel/AbstractTNCCSProtocol.cpp)
set(src ${src} src/tncs/iel/Policy.cpp)
set(src ${src} src/tncs/iel/Coordinator.cpp)
set(src ${src} src/tncs/iel/Connection.cpp)
set(src ${src} src/tncs/iel/IMV.cpp)
set(src ${src} src/tncs/iel/IMVProperties.cpp)
set(src ${src} src/tncs/iel/IFTNCSImpl.cpp)
set(src ${src} src/tncs/exception/ConnectionStateException.cpp)
set(src ${src} src/tncs/exception/ConnectionStateExceptionWithTNCCSData.cpp)
set(src ${src} src/tncs/exception/ResultException.cpp)
set(src ${src} src/tncs/exception/TNCCSProtocolNotSupportedException.cpp)
set(src ${src} src/tncs/iel/tnc1/TNCCTNCSMessageBase64.cpp)
set(src ${src} src/tncs/iel/tnc1/TNCCTNCSMessage.cpp)
set(src ${src} src/tncs/iel/tnc1/TNCCTNCSMessageXML.cpp)
set(src ${src} src/tncs/iel/tnc1/IMCIMVMessage.cpp)
set(src ${src} src/tncs/iel/tnc1/TNCCSBatch.cpp)
set(src ${src} src/tncs/iel/tnc1/TNC1Protocol.cpp)

# define shared object
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries
target_link_libraries(${PROJECT_NAME} ${LTDL_LIBRARY} ${XercesC_LIBRARY} ${Log4cxx_LIBRARY} tncutil tncxacml)

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install target
install(DIRECTORY src/tncs 
        DESTINATION include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.svn*" EXCLUDE)
                  
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tncsConfig.h
        DESTINATION include/tncs)
IF(NOT EXISTS ${TNCS_CONFIG_DIR}/tnc_config)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/tnc_config
        DESTINATION ${TNCS_CONFIG_DIR})
ENDIF()
IF(NOT EXISTS ${TNCS_CONFIG_DIR}/tnc/xacml/xacml-ifm-imv.conf)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/xacml-ifm-imv.conf
        DESTINATION ${TNCS_CONFIG_DIR}/tnc/xacml)
ENDIF()

 
install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

