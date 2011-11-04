# Find the native XercesC includes and library
#
# This module defines
#  XercesC_INCLUDE_DIRS, where to find xercesc headers
#  XercesC_LIBRARIES, where to find the xercesc library.
#  XercesC_FOUND, If false, do not try to use XercesC.

FIND_PATH(XercesC_INCLUDE_DIR xercesc)

FIND_LIBRARY(XercesC_LIBRARY xerces-c) 

IF(XercesC_INCLUDE_DIR)
    FILE(READ "${XercesC_INCLUDE_DIR}/xercesc/util/XercesVersion.hpp" _XercesC_VERSION_H_CONTENTS)
    
    STRING(REGEX REPLACE ".*#define XERCES_VERSION_MAJOR *([0-9]+).*" "\\1" XercesC_VERSION_MAJOR "${_XercesC_VERSION_H_CONTENTS}")
    STRING(REGEX REPLACE ".*#define XERCES_VERSION_MINOR *([0-9]+).*" "\\1" XercesC_VERSION_MINOR "${_XercesC_VERSION_H_CONTENTS}")
    STRING(REGEX REPLACE ".*#define XERCES_VERSION_REVISION *([0-9]+).*" "\\1" XercesC_VERSION_PATCH "${_XercesC_VERSION_H_CONTENTS}")

    SET(XercesC_VERSION "${XercesC_VERSION_MAJOR}.${XercesC_VERSION_MINOR}.${XercesC_VERSION_PATCH}")
ENDIF()

IF (XercesC_INCLUDE_DIR AND XercesC_LIBRARY)
    SET(XercesC_FOUND TRUE)
ENDIF (XercesC_INCLUDE_DIR AND XercesC_LIBRARY)

IF(XercesC_FOUND)
    IF(XercesC_FIND_VERSION_MAJOR AND NOT XercesC_FIND_VERSION_MAJOR EQUAL "${XercesC_VERSION_MAJOR}")
        IF(XercesC_FIND_VERSION_EXACT OR XercesC_FIND_VERSION_MAJOR GREATER "${XercesC_VERSION_MAJOR}")
            SET(XercesC_FOUND FALSE)
        ENDIF()
    ELSE()
        IF(XercesC_FIND_VERSION_MINOR AND NOT XercesC_FIND_VERSION_MINOR EQUAL "${XercesC_VERSION_MINOR}")
            IF(XercesC_FIND_VERSION_EXACT OR XercesC_FIND_VERSION_MINOR GREATER "${XercesC_VERSION_MINOR}")
                SET(XercesC_FOUND FALSE)
            ENDIF()
        ELSE()
            IF(XercesC_FIND_VERSION_PATCH AND NOT XercesC_FIND_VERSION_PATCH EQUAL "${XercesC_VERSION_PATCH}")
                IF(XercesC_FIND_VERSION_EXACT OR XercesC_FIND_VERSION_PATCH GREATER "${XercesC_VERSION_PATCH}")
                    SET(XercesC_FOUND FALSE)
                ENDIF()
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF(XercesC_FOUND)


IF (XercesC_FOUND)
    LIST(APPEND XercesC_INCLUDE_DIRS "${XercesC_INCLUDE_DIR}")
    LIST(APPEND XercesC_LIBRARIES "${XercesC_LIBRARY}")

   IF (NOT XercesC_FIND_QUIETLY)
      MESSAGE(STATUS "Found XercesC: ${XercesC_LIBRARY} (${XercesC_VERSION})")
   ENDIF (NOT XercesC_FIND_QUIETLY)
ELSE (XercesC_FOUND)
   IF (NOT XercesC_INCLUDE_DIR AND NOT XercesC_LIBRARY)
       SET(TMP_REASON "Include and Library not found.")
   ELSEIF(XercesC_INCLUDE_DIR AND NOT XercesC_LIBRARY)
       SET(TMP_REASON "Library not found.")
   ELSEIF(NOT XercesC_INCLUDE_DIR AND XercesC_LIBRARY)
       SET(TMP_REASON "Include not found.")
   ELSE (NOT XercesC_INCLUDE_DIR AND NOT XercesC_LIBRARY)
       SET(TMP_REASON "Version ${XercesC_FIND_VERSION} not found.")
   ENDIF(NOT XercesC_INCLUDE_DIR AND NOT XercesC_LIBRARY)

   IF (XercesC_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find XercesC. Reason: ${TMP_REASON}")
   ELSE (XercesC_FIND_REQUIRED)
      MESSAGE(STATUS "Could not find XercesC. Reason: ${TMP_REASON}")
   ENDIF (XercesC_FIND_REQUIRED)
ENDIF (XercesC_FOUND)
