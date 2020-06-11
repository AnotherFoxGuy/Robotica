#pragma once

#include <window/main_window.hpp>
#include <world/robot.hpp>

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>

namespace robotica
{
inline void joystick_callback(std::string_view argstring)
{
    const static std::regex rgx{R"REGEX(\s*(-?\d+)\s*,\s*(-?\d+)\s*)REGEX"};
    auto args = regex_groups(std::string(argstring), rgx);

    auto& settings = main_window::instance();
    int x = std::stoi(args[1]) + std::stoi(args[2]);
    int y = std::stoi(args[2]) - std::stoi(args[1]);
    settings.left_motor = 0.01 * std::clamp(x, -100, 100);
    settings.right_motor = 0.01 * std::clamp(y, -100, 100);

    std::cout << "Robot movement speed was changed by joystick input.\n";
}

inline void speed_callback(std::string_view argstring)
{
    const static std::regex rgx { R"REGEX((-?\d+))REGEX" };
    auto args = regex_groups(std::string(argstring), rgx);

    auto& settings = main_window::instance();
    settings.speed = std::stoi(args[0]);

    std::cout << "speed input.\n";
}
} // namespace robotica
