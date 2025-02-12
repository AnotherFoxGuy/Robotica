include(SourceFileUtils)
include(cotire)

project(RBTC)

####################################################################################################
#  SOURCE FILES
####################################################################################################

set(SOURCE_FILES

     dance/AudioFile.h
     dance/MusicAnalyzer.{hpp,cpp}
     dance/strategies/idance_strategy.hpp
     dance/strategies/dance_moves.{hpp,cpp}
     comms/websocket.hpp
     comms/joystick_callback.hpp
     comms/gripper_callback.hpp
     comms/strategy_callback.hpp
     utility/cache.hpp
     utility/math.hpp
     utility/traits.hpp
     utility/typedefs.hpp
     utility/utility.hpp
     vision/cascade_classifier.hpp
     vision/pool_classifier.hpp
     vision/iclassifier.hpp
     vision/world_model.{hpp,cpp}
     vision/converters.hpp
     vision/floodfill.hpp
     vision/heat_measure.hpp
     vision/parallax.hpp
     window/window.{hpp,cpp}
     window/main_window.{hpp,cpp}
     window/setting.hpp
     window/gui_image.hpp
     window/world_image.hpp
     window/imgui_bindings.{hpp,cpp}
     window/imgui_plot/imgui_plot.{hpp,cpp}
     window/3d/buffer.hpp
     window/3d/primitive.hpp
     window/3d/program.hpp
     world/controller.{hpp,cpp}
     world/robot.{hpp,cpp}
     world/strategy/istrategy.hpp
     world/strategy/strategy_playingcards.hpp
     world/strategy/strategy_dance.hpp
     main.cpp
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