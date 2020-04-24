include(SourceFileUtils)

project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES
        main.cpp
        WebViewManager.{h,cpp}
        )

# Expand file extensions (i.e. path/to/file.{h,cpp} becomes path/to/file.h;path/to/file.cpp)
expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC .)

if (UNIX)
    set(LINK_LIBS dl rt)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LINK_LIBS}
        Threads::Threads
        CONAN_PKG::webots-controller
        CONAN_PKG::opencv
        CONAN_PKG::poco
        )
