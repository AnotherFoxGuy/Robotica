include(FindPackageHandleStandardArgs)

####################################################################################################
#  Default (c)
####################################################################################################

find_path(Webots_C_INCLUDE_DIRS webots/Lidar.h
        HINTS $ENV{WEBOTS_HOME}/include/controller/c/
        )

find_library(Webots_C_LIBRARIES
        NAMES Controller libController
        HINTS $ENV{WEBOTS_HOME}/lib/controller
        )

find_package_handle_standard_args(Webots FOUND_VAR Webots_FOUND
        REQUIRED_VARS Webots_C_INCLUDE_DIRS Webots_C_LIBRARIES
        )

if (Webots_FOUND)
    add_library(Webots::Controller INTERFACE IMPORTED)
    set_target_properties(Webots::Controller PROPERTIES
            INTERFACE_LINK_LIBRARIES "${Webots_C_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${Webots_C_INCLUDE_DIRS}"
            )
endif ()
mark_as_advanced(Webots_C_LIBRARIES Webots_C_INCLUDE_DIRS)