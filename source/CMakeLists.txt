include(SourceFileUtils)
include(cotire)

project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES
        main.cpp
        WebViewManager.{h,cpp}
        world/robot.hpp
        world/world.hpp
        window/gui_image.hpp
        window/main_window.hpp
        window/main_window.cpp
        window/imgui_bindings.{h,cpp}
        window/setting.hpp
        window/window.hpp
        vision/converters.hpp
        vision/parallax.hpp
        utility/traits.hpp
        utility/typedefs.hpp
        )

# Expand file extensions (i.e. path/to/file.{h,cpp} becomes path/to/file.h;path/to/file.cpp)
expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_include_directories(${PROJECT_NAME} PUBLIC . ${CONAN_RES_DIRS_IMGUI}/bindings/)

if (NOT "_$ENV{CI}" STREQUAL "_")
    message("Shit Fix")
    set(SHIT_FIX "-Wl,--no-as-needed")
endif ()


target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SHIT_FIX}
        ${CMAKE_DL_LIBS}
        Threads::Threads
        CONAN_PKG::webots-controller
        CONAN_PKG::opencv
        CONAN_PKG::poco
        CONAN_PKG::glm
        CONAN_PKG::imgui
        CONAN_PKG::sdl2
        CONAN_PKG::glew
        )


add_compile_definitions("ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"")
add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX SDL_MAIN_HANDLED)

cotire(${PROJECT_NAME})