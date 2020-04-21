include(FindPackageHandleStandardArgs)

####################################################################################################
#  cpp bindings
####################################################################################################

find_path(Webots_CPP_INCLUDE_DIRS webots/Lidar.hpp
        HINTS $ENV{WEBOTS_HOME}/include/controller/cpp/
        )

find_library(Webots_CPP_LIBRARIES
        NAMES CppController libCppController
        HINTS $ENV{WEBOTS_HOME}/lib/controller
        )

if ("${Webots_CPP_LIBRARIES}" MATCHES "NOTFOUND")
    set(Webots_CPP_LIBRARIES "CppController")

    find_path(Webots_CPP_LIB_DIR DifferentialWheels.cpp
            HINTS $ENV{WEBOTS_HOME}/resources/languages/cpp/
            )

    file(GLOB Webots_CPP_LIB_SOURCE "${Webots_CPP_LIB_DIR}/*.cpp")

    include_directories($ENV{WEBOTS_HOME}/include/controller/c $ENV{WEBOTS_HOME}/include/controller/cpp)
    add_library(${Webots_CPP_LIBRARIES} ${Webots_CPP_LIB_SOURCE})

    set_target_properties(${Webots_CPP_LIBRARIES} PROPERTIES FOLDER HiddenTargets)
endif ()

find_package_handle_standard_args(Webots_CPP FOUND_VAR Webots_CPP_FOUND
        REQUIRED_VARS Webots_CPP_INCLUDE_DIRS Webots_CPP_LIBRARIES
        )

if (Webots_CPP_FOUND)
    add_library(Webots::CppController INTERFACE IMPORTED)
    set_target_properties(Webots::CppController PROPERTIES
            INTERFACE_LINK_LIBRARIES "${Webots_CPP_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${Webots_CPP_INCLUDE_DIRS}"
            )
endif ()

mark_as_advanced(Webots_CPP_LIBRARIES Webots_CPP_INCLUDE_DIRS Webots_CPP_LIB_SOURCE)