#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(vpn)
MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")
#library headers here
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libtncvpn/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncutil/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncs/src/)

# Find boost
#SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0")
SET(BOOST_ROOT "/usr/include/boost/")
find_package( Boost 1.38.0 COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

IF(${COMPONENT} STREQUAL "AR" OR ${COMPONENT} STREQUAL "PDP" OR ${COMPONENT} STREQUAL "PEP" OR ${COMPONENT} STREQUAL "ALL")
	add_subdirectory(libtncvpn)
ENDIF(${COMPONENT} STREQUAL "AR" OR ${COMPONENT} STREQUAL "PDP" OR ${COMPONENT} STREQUAL "PEP" OR ${COMPONENT} STREQUAL "ALL")

IF(${COMPONENT} STREQUAL "AR" OR ${COMPONENT} STREQUAL "ALL")
	add_subdirectory(vpnclient)
ENDIF(${COMPONENT} STREQUAL "AR" OR ${COMPONENT} STREQUAL "ALL")

IF(${COMPONENT} STREQUAL "PDP" OR ${COMPONENT} STREQUAL "ALL")
	add_subdirectory(naavpn)
ENDIF(${COMPONENT} STREQUAL "PDP" OR ${COMPONENT} STREQUAL "ALL")

IF(${COMPONENT} STREQUAL "PEP" OR ${COMPONENT} STREQUAL "ALL")
	add_subdirectory(vpnpep)
ENDIF(${COMPONENT} STREQUAL "PEP" OR ${COMPONENT} STREQUAL "ALL")
