cmake_minimum_required(VERSION 2.6)
project(tncfhh)

# project version
SET(TNCFHH_VERSION_MAJOR 0)
SET(TNCFHH_VERSION_MINOR 8)
SET(TNCFHH_VERSION_PATCH 4)

# build options
SET(COMPONENT	"NOTHING" CACHE BOOL "Set to AR, PDP, PEP or TNCSIM to choose the system to build the components for.")
SET(NAL		"NOTHING" CACHE BOOL "Set to 8021x or VPN to choose the underlying network layer technology.")
SET(TPM		"OFF" CACHE BOOL "Set to ON when using the TPM")

#to avoid hardcoded directory paths
IF(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	SET(CMAKE_INSTALL_LIBDIR "lib")
ENDIF(NOT DEFINED CMAKE_INSTALL_LIBDIR)
IF(NOT DEFINED SYSCONF_INSTALL_DIR)
	SET(SYSCONF_INSTALL_DIR "/etc")
ENDIF(NOT DEFINED SYSCONF_INSTALL_DIR)

# convert variables to uppercase
string(TOUPPER ${COMPONENT} COMPONENT)
string(TOUPPER ${NAL} NAL)

# includes for shared headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared/include)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shared/cmake_modules")

# configure a header file to pass some of the CMake settings
# to the source code - primarily used for versioning
configure_file (
  "${PROJECT_SOURCE_DIR}/tncfhhConfig.h.in"
  "${PROJECT_BINARY_DIR}/tncfhhConfig.h"
  )
# add the binary tree to the search path for include files
# so that we will find tncfhhConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# log4cxx
find_package(Log4cxx REQUIRED)
include_directories(${Log4cxx_INCLUDE_DIR})

# add subdirectories depending on build options
IF(${COMPONENT} STREQUAL "AR")
	SET(TNCFHH_BUILD_IMCS "ON" CACHE BOOL "")

	add_subdirectory(imunit)
	add_subdirectory(tncutil)
	add_subdirectory(tncxacml)

	# only IMCs
	add_subdirectory(imcv)
	
	IF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(tncs)
		add_subdirectory(vpn)
	ENDIF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
ENDIF(${COMPONENT} STREQUAL "AR")

IF(${COMPONENT} STREQUAL "PEP")
	IF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(tncutil)
		add_subdirectory(vpn)
	ENDIF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
ENDIF(${COMPONENT} STREQUAL "PEP")

IF(${COMPONENT} STREQUAL "PDP")
	SET(TNCFHH_BUILD_IMVS "ON" CACHE BOOL "")

	add_subdirectory(imunit)
	add_subdirectory(tncutil)
	add_subdirectory(tncxacml)
	
	# only IMVs
	add_subdirectory(imcv)
	
	add_subdirectory(tncs)
	
	IF(${NAL} STREQUAL "8021X" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(naaeap)
	ENDIF(${NAL} STREQUAL "8021X" OR ${NAL} STREQUAL "ALL")
	
	IF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(vpn)
	ENDIF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
ENDIF(${COMPONENT} STREQUAL "PDP")

IF(${COMPONENT} STREQUAL "TNCSIM")
	add_subdirectory(tncs)
	add_subdirectory(naaeap)
	add_subdirectory(tncsim)
ENDIF(${COMPONENT} STREQUAL "TNCSIM")

IF(${COMPONENT} STREQUAL "ALL")
	SET(TNCFHH_BUILD_IMCS "ON" CACHE BOOL "")
	SET(TNCFHH_BUILD_IMVS "ON" CACHE BOOL "")

	add_subdirectory(imunit)
	add_subdirectory(tncutil)
	add_subdirectory(tncxacml)
	add_subdirectory(imcv)
	add_subdirectory(tncs)
	add_subdirectory(tncsim)
			
	IF(${NAL} STREQUAL "8021X" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(naaeap)
		include_directories(${CMAKE_CURRENT_SOURCE_DIR}/naaeap/src)
	ENDIF(${NAL} STREQUAL "8021X" OR ${NAL} STREQUAL "ALL")
	
	IF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
		add_subdirectory(vpn)
	ENDIF(${NAL} STREQUAL "VPN" OR ${NAL} STREQUAL "ALL")
ENDIF(${COMPONENT} STREQUAL "ALL")

# define install target
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tncfhhConfig.h DESTINATION include)                  
install(DIRECTORY shared/include/tcg DESTINATION include
                  FILES_MATCHING
                  PATTERN "*.h"
                  PATTERN "*.svn*" EXCLUDE)

# build a CPack driven installer package
#include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_IGNORE_FILES
	"/.svn/"
	".swp$"
	"/build/"
	".project$"
	".cproject$"
#	"/doc/design/"
)
set (CPACK_PACKAGE_VERSION_MAJOR "${TNCFHH_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${TNCFHH_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${TNCFHH_VERSION_PATCH}")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}")
include (CPack)

# print variables
MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")
MESSAGE(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "-------------------------------------------------------------------------------" )
MESSAGE(STATUS "CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
MESSAGE(STATUS "CPACK_SOURCE_PACKAGE_FILE_NAME = ${CPACK_SOURCE_PACKAGE_FILE_NAME}")
MESSAGE(STATUS "-------------------------------------------------------------------------------" )
MESSAGE(STATUS "COMPONENT = ${COMPONENT}")
MESSAGE(STATUS "NAL TECHNOLOGY = ${NAL}")
MESSAGE(STATUS "USING TPM = ${TPM}")
MESSAGE(STATUS "-------------------------------------------------------------------------------" )
MESSAGE(STATUS "Change a value with: cmake -D<Variable>=<Value> <path/to/src>" )
MESSAGE(STATUS "            example: cmake -DCMAKE_BUILD_TYPE=Debug           \\ " )
MESSAGE(STATUS "                           -DCMAKE_CXX_FLAGS_DEBUG=\"-g -Wall\" \\ " )
MESSAGE(STATUS "                           -DCMAKE_VERBOSE_MAKEFILE=\"ON\"      \\ " )
MESSAGE(STATUS "                           ../.." )
MESSAGE(STATUS "-------------------------------------------------------------------------------" )

