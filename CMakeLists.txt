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

pmm(CONAN REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)

set(CMAKE_CXX_STANDARD 20)

get_variable_from_conan(CONAN_RES_DIRS_IMGUI)

add_subdirectory(source)