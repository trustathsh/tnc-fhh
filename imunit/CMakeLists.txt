#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

project(imunit)

MESSAGE(STATUS "")
MESSAGE(STATUS "-- ${PROJECT_NAME} --")

set(src)
set(src ${src} src/imunit/IMUnitLibrary.cpp)
set(src ${src} src/imunit/exception/ResultException.cpp)
set(src ${src} src/imunit/AbstractIMUnit.cpp)
set(src ${src} src/imunit/imv/IMVLibrary.cpp)
set(src ${src} src/imunit/imv/TNCS.cpp)
set(src ${src} src/imunit/imv/AbstractIMV.cpp)
set(src ${src} src/imunit/imc/IMCLibrary.cpp)
set(src ${src} src/imunit/imc/AbstractIMC.cpp)
set(src ${src} src/imunit/imc/TNCC.cpp)

# ensure PIC
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
endif( CMAKE_COMPILER_IS_GNUCXX )

#define library
add_library(${PROJECT_NAME} SHARED ${src})

# add libraries needed by imunit
target_link_libraries(${PROJECT_NAME} ${Log4cxx_LIBRARY})

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install target
install(DIRECTORY src/ DESTINATION include
                  FILES_MATCHING
                  PATTERN "*.h"
                  PATTERN "*.svn*" EXCLUDE)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
