#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(naaeap)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tncs/src/)

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/naaeap/data/EapTncFragment.cpp)
set(src ${src} src/naaeap/data/EapTncBuffer.cpp)
set(src ${src} src/naaeap/naaeap.cpp)
set(src ${src} src/naaeap/exception/AcknowledgementException)
set(src ${src} src/naaeap/logic/Fragmentation.cpp)
set(src ${src} src/naaeap/logic/Defragmentation.cpp)

# define shared object
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries
target_link_libraries(${PROJECT_NAME} ${Log4cxx_LIBRARY} tncs)

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install targets
install(DIRECTORY src/naaeap DESTINATION include
                  FILES_MATCHING
                  PATTERN "*.h"
                  PATTERN "*.svn*" EXCLUDE)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
