include(SourceFileUtils)
include(cotire)

project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES
     main.cpp
     dance/AudioFile.h
     dance/MusicAnalyzer.{hpp,cpp}
     world/controller.{hpp,cpp}
     world/robot.{hpp,cpp}
     window/gui_image.hpp
     window/imgui_bindings.{hpp,cpp}
     window/main_window.{hpp,cpp}
     window/setting.hpp
     window/window.hpp
     window/world_image.hpp
     window/imgui_plot/imgui_plot.{hpp,cpp}
     vision/iclassifier.hpp
     vision/cascade_classifier.hpp
     vision/pool_classifier.hpp
     vision/converters.hpp
     vision/parallax.hpp
     vision/world_model.{hpp,cpp}
     utility/cache.hpp
     utility/math.hpp
     utility/mesh.hpp
     utility/traits.hpp
     utility/typedefs.hpp
     utility/utility.hpp
)

# Expand file extensions (i.e. path/to/file.{hpp,cpp} becomes path/to/file.hpp;path/to/file.cpp)
expand_file_extensions(SOURCE_FILES ${SOURCE_FILES})

# Generate source groups for use in IDEs
generate_source_groups(${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

get_variable_from_conan(CONAN_RES_DIRS_IMGUI)
target_include_directories(${PROJECT_NAME} PUBLIC . ${CONAN_RES_DIRS_IMGUI}/bindings/)

target_link_libraries(${PROJECT_NAME} PRIVATE
     "CRYPT32"
     CONAN_PKG::webots-controller
     CONAN_PKG::opencv
     CONAN_PKG::ixwebsocket
     CONAN_PKG::glm
     CONAN_PKG::imgui
     CONAN_PKG::sdl2
     CONAN_PKG::glew
     CONAN_PKG::magic_enum
)


add_compile_definitions("ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"")
add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX SDL_MAIN_HANDLED)

target_compile_definitions(${PROJECT_NAME} PUBLIC USING_EASY_PROFILER IMGUI_DEFINE_MATH_OPERATORS)
target_link_libraries(${PROJECT_NAME} PRIVATE CONAN_PKG::easy_profiler)

cotire(${PROJECT_NAME})