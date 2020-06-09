#pragma once

#include <world/robot.hpp>
#include <window/main_window.hpp>

#include <string_view>
#include <regex>
#include <string>
#include <algorithm>
#include <iostream>

namespace robotica {
    inline void arm_base_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX(\s*(-?\d+)\s*,\s*(-?\d+)\s*)REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.arm_base  = 0.005 * std::clamp(std::stoi(args[1]), -100, 100);

        std::cout << "Robot arm base was changed by joystick input.\n";
    }
}
