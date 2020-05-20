cmake_minimum_required(VERSION 3.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
project("Robotica")

find_package(Threads REQUIRED)

include(cmakepp)

function(get_variable_from_conan _var_name)
     include("${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")
     get_property(__propt VARIABLE PROPERTY ${_var_name})
     set(${_var_name} ${__propt} PARENT_SCOPE)
endfunction()

SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
SET(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_USE_RELATIVE_PATHS OFF)

template_run_file("conanfile.txt.in")
ans(gen_conan)
fwrite("conanfile.txt" "${gen_conan}")

include(pmm)

if (MSVC AND "_${CMAKE_BUILD_TYPE}" STREQUAL "_Debug")
     # MSVC needs extra settings because glew only compiles with VC2017
     # Also Poco has to be explicitly told this is a debug build for some reason.
     pmm(CONAN PROFILE "${CMAKE_SOURCE_DIR}/msvc_debug.conanprofile" REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)
else()
     pmm(CONAN REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)
endif()

set(CMAKE_CXX_STANDARD 20)



get_variable_from_conan(CONAN_RES_DIRS_IMGUI)

add_subdirectory(source)