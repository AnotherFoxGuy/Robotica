#pragma once

#include <world/robot.hpp>
#include <window/main_window.hpp>

#include <string_view>
#include <regex>
#include <string>
#include <algorithm>
#include <iostream>


namespace robotica {
    inline void joystick_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX(\s*(-?\d+)\s*,\s*(-?\d+)\s*)REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.left_motor  = std::clamp(std::stoi(args[1]), -100, 100);
        settings.right_motor = std::clamp(std::stoi(args[2]), -100, 100);

        std::cout << "Robot movement speed was changed by joystick input.\n";
    }
}