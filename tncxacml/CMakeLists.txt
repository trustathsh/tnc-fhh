#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(tncxacml)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncutil/src/)

# BOOST_ROOT ALS prefix definieren
SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0")
SET(BOOST_ROOT "/usr/local/include/boost/boost_1_39_0/")
find_package( Boost 1.39.0 COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(XercesC 2.8 REQUIRED)
include_directories(${XercesC_INCLUDE_DIR})

set(src)
set(src ${src} src/tncxacml/XACMLHelper.cpp)
set(src ${src} src/tncxacml/XACMLConstants.cpp)

# ensure PIC
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
endif( CMAKE_COMPILER_IS_GNUCXX )

#define library
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries needed by tncxacml
target_link_libraries(${PROJECT_NAME} ${Log4cxx_LIBRARY} ${XercesC_LIBRARY} -lssl ${Boost_LIBRARIES} tncutil)

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install target
install(DIRECTORY src/ DESTINATION include
                  FILES_MATCHING
                  PATTERN "*.h"
                  PATTERN "*.svn*" EXCLUDE)

SET(CONFIG_FILE xacml-pdp.conf)
IF(NOT EXISTS ${SYSCONF_INSTALL_DIR}/tnc/xacml/${CONFIG_FILE})
	install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/conf/${CONFIG_FILE}
		DESTINATION ${SYSCONF_INSTALL_DIR}/tnc/xacml)
ENDIF(NOT EXISTS ${SYSCONF_INSTALL_DIR}/tnc/xacml/${CONFIG_FILE})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
