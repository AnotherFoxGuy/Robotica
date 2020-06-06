cmake_minimum_required(VERSION 3.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
project("Robotica")

function(get_variable_from_conan _var_name)
     include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
     get_property(__propt VARIABLE PROPERTY ${_var_name})
     set(${_var_name} ${__propt} PARENT_SCOPE)
endfunction()

SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
SET(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_USE_RELATIVE_PATHS OFF)


include(pmm)

# MSVC needs extra settings because glew only compiles with VC2017
# Also Poco has to be explicitly told this is a debug build for some reason.
if (MSVC)
     if ("_${CMAKE_BUILD_TYPE}" STREQUAL "_Debug")
          set(CONAN_PROFILE "${CMAKE_SOURCE_DIR}/msvc_debug.conanprofile")
     else()
          set(CONAN_PROFILE "${CMAKE_SOURCE_DIR}/msvc_release.conanprofile")
     endif()
else()
     # Clang / GCC profiles go here.
     set(CONAN_PROFILE "default")
endif()

pmm(CONAN PROFILE ${CONAN_PROFILE} REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)

set(CMAKE_CXX_STANDARD 20)

get_variable_from_conan(CONAN_RES_DIRS_IMGUI)

add_subdirectory(source)