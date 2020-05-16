cmake_minimum_required(VERSION 3.12)
project("Robotica")

find_package(Threads REQUIRED)

SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
SET(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_USE_RELATIVE_PATHS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(pmm)

if (MSVC)
     # MSVC needs extra settings because glew only compiles with VC2017
     pmm(CONAN PROFILE "${CMAKE_SOURCE_DIR}/msvc.conanprofile" REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)
else()
     pmm(CONAN REMOTES AnotherFoxGuy https://api.bintray.com/conan/anotherfoxguy/conan-packages BINCRAFTERS CMakeCM ROLLING)
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(source)
