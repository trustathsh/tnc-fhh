# Version handling
# this macro calc the variables:
#                               ${PREFIX}_VERSION_SUPPLEMENT
#                               ${PREFIX}_VERSION
#
# you must set the variables:
#                               ${PREFIX}_VERSION_MAJOR
#                               ${PREFIX}_VERSION_MINOR
#                               ${PREFIX}_VERSION_PATCH
#                               ${PREFIX}_VERSION_TWEAK
#                               ${PREFIX}_VERSION_SUPPLEMENT
#
# example: calc_version("TNCS")
#
# written by Mike Steinmetz (mbs)
# Date: 2009-07-11

macro(calc_version PREFIX)
    set(${PREFIX}_VERSION "${${PREFIX}_VERSION_MAJOR}.${${PREFIX}_VERSION_MINOR}.${${PREFIX}_VERSION_PATCH}")

    if(${PREFIX}_VERSION_TWEAK)
        set(${PREFIX}_VERSION "${${PREFIX}_VERSION}.${${PREFIX}_VERSION_TWEAK}")
    endif(${PREFIX}_VERSION_TWEAK)
    
    if(EXISTS "${PROJECT_SOURCE_DIR}/.svn")
        find_package(Subversion)
        if(Subversion_FOUND)
            Subversion_WC_INFO(${PROJECT_SOURCE_DIR} SVN_${PREFIX})
            if(SVN_${PREFIX}_WC_REVISION)
                message(STATUS "Current SVN revision is ${SVN_${PREFIX}_WC_REVISION}.")
            else(SVN_${PREFIX}_WC_REVISION)
                message(STATUS "SVN repository found, but could not found compatible SVN program.")
            endif(SVN_${PREFIX}_WC_REVISION)
        else(Subversion_FOUND)
            message(STATUS "SVN repository found, but could not found SVN program.")
        endif(Subversion_FOUND)
        if(${PREFIX}_VERSION_SUPPLEMENT)
            set(${PREFIX}_VERSION_SUPPLEMENT "${${PREFIX}_VERSION_SUPPLEMENT}-svn${SVN_${PREFIX}_WC_REVISION}")
        else(${PREFIX}_VERSION_SUPPLEMENT)
            set(${PREFIX}_VERSION_SUPPLEMENT "svn${SVN_${PREFIX}_WC_REVISION}")
        endif(${PREFIX}_VERSION_SUPPLEMENT)
    endif(EXISTS "${PROJECT_SOURCE_DIR}/.svn")
   
    if(${PREFIX}_VERSION_SUPPLEMENT)
        set(${PREFIX}_VERSION "${${PREFIX}_VERSION}-${${PREFIX}_VERSION_SUPPLEMENT}")
    endif(${PREFIX}_VERSION_SUPPLEMENT)

endmacro(calc_version)
