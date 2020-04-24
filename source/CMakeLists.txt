include(SourceFileUtils)

project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES
        main.cpp
        world/robot.hpp
        world/world.hpp
        utility/math.hpp
        utility/typedefs.hpp
        WebViewManager.{h,cpp}
        )

# Expand file extensions (i.e. path/to/file.{h,cpp} becomes path/to/file.h;path/to/file.cpp)
expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC .)


target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_DL_LIBS}
        Threads::Threads
        CONAN_PKG::webots-controller
        CONAN_PKG::opencv
        CONAN_PKG::poco
        CONAN_PKG::glm
        )
