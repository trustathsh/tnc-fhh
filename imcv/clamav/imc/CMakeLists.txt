#=====================================================
#  _____                _     ____  _____ _   _ _   _ 
# |_   _|_ __ _   _ ___| |_  / __ \|  ___| | | | | | |
#   | | | '__| | | / __| __|/ / _` | |_  | |_| | |_| |
#   | | | |  | |_| \__ \ |_| | (_| |  _| |  _  |  _  |
#   |_| |_|   \__,_|___/\__|\ \__,_|_|   |_| |_|_| |_|
#                            \____/                   
#=====================================================

set(TYPE imc)

MESSAGE( STATUS "" )
MESSAGE( STATUS "-- ${PROJECT_NAME}${TYPE} --" )

#set imc config file
set(IMC_CONFIG_FILE  "clamavimc.conf")

#set imc config dir
set(IMC_CONFIG_DIR  "${SYSCONF_INSTALL_DIR}/tnc")

#set imc config
set(IMC_CONFIG  "${IMC_CONFIG_DIR}/${IMC_CONFIG_FILE}")

# print configuration information
message(STATUS "IMC_CONFIG_FILE = ${IMC_CONFIG_FILE}")
message(STATUS "IMC_CONFIG_DIR = ${IMC_CONFIG_DIR}")
message(STATUS "IMC_CONFIG = ${IMC_CONFIG}")

# configuration header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../${TYPE}Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${TYPE}Config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# set sourcecode
# got with: $ find src \( -iname "*.cpp" -o -iname "*.c" \) -exec echo "set(src \${src} {})" \;
set(src)
set(src ${src} src/ClamavIMCLibrary.cpp)
set(src ${src} src/ClamavIMC.cpp)

# define shared object
add_library(${PROJECT_NAME}${TYPE} MODULE ${src})

# add libraries
target_link_libraries(${PROJECT_NAME}${TYPE} imunit)

# set version
SET_TARGET_PROPERTIES(${PROJECT_NAME}${TYPE} PROPERTIES VERSION ${TNCFHH_VERSION_MAJOR}.${TNCFHH_VERSION_MINOR}.${TNCFHH_VERSION_PATCH}  SOVERSION 0)

# define install target
IF(NOT EXISTS ${IMC_CONFIG})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/conf/${IMC_CONFIG_FILE}
        DESTINATION ${IMC_CONFIG_DIR})
ENDIF()

install(TARGETS ${PROJECT_NAME}${TYPE} DESTINATION ${CMAKE_INSTALL_LIBDIR})
