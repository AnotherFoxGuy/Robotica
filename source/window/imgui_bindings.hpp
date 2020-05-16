#pragma once

// Unfortunately the conan package does not provide a direct path to the ImGUI bindings...
// TODO: Check https://github.com/conan-io/conan-center-index/issues/749 to see if this has been fixed.
#include <imgui.h/../../res/bindings/imgui_impl_opengl3.h>
#include <imgui.h/../../res/bindings/imgui_impl_sdl.h>