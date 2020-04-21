cmake_minimum_required(VERSION 3.12)
project("Robotica")

SET(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
SET(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(FeatureSummary)

find_package(Webots REQUIRED)
find_package(Webots_CPP REQUIRED)

add_subdirectory(source)



feature_summary(WHAT ALL)