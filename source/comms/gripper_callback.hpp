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
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.arm_base  = std::stof(args[0]);

        std::cout << "Robot arm base was changed by joystick input.\n";
    }
    inline void arm_short_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.arm_short  = std::stof(args[0]);

        std::cout << "Robot arm short was changed by joystick input.\n";
    }
    inline void arm_long_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.arm_long  = std::stof(args[0]);

        std::cout << "Robot arm long was changed by joystick input.\n";
    }
    inline void gripper_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.gripper  = std::stof(args[0]);
        //settings.gripper  = std::clamp(std::stoi(args[0]), -100, 100);

        std::cout << "Robot gripper was changed by joystick input.\n";
    }
    inline void gripper_pitch_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.gripper_pitch  =std::stof(args[0]);

        std::cout << "Robot gripper pitch was changed by joystick input.\n";
    }
    inline void gripper_roll_callback(std::string_view argstring) {
        const static std::regex rgx { R"REGEX((-?\d+\.*\d*))REGEX" };
        auto args = regex_groups(std::string(argstring), rgx);

        auto& settings = main_window::instance();
        settings.gripper_roll  = std::stof(args[0]);

        std::cout << "Robot gripper roll was changed by joystick input.\n";
    }
}
