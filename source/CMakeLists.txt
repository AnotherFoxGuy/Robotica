project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES main.cpp)

include(SourceFileUtils)

# Expand file extensions (i.e. path/to/file.{h,cpp} becomes path/to/file.h;path/to/file.cpp)
expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE Webots::Controller Webots::CppController)

