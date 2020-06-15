#pragma once

#include <window/main_window.hpp>
#include <world/robot.hpp>

#include <iostream>
#include <string>

namespace robotica
{
const static int autonomousSpeed = 30;
inline auto get_regex(std::string_view argString)
{
    const static std::regex rgx{R"REGEX((-?\d+))REGEX"};
    return regex_groups(std::string(argString), rgx);
}
inline void start_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Selected robot mode has been started.\n";
}
inline void stop_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Selected robot mode has been stopped.\n";
}
inline void manual_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Robot mode was changed to manual.\n";
}
inline void moonVision_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Robot mode was changed to moon vision.\n";
}
inline void moonDance_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Robot mode was changed to moon dance.\n";
}
inline void moonWalk_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    auto& settings = main_window::instance();
    settings.speed = autonomousSpeed;

    std::cout << "Robot mode was changed to moon walk.\n";
}
inline void moonSurvival_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    auto& settings = main_window::instance();
    settings.speed = autonomousSpeed;

    std::cout << "Robot mode was changed to moon survival.\n";
}
inline void moonMaze_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    auto& settings = main_window::instance();
    settings.speed = autonomousSpeed;

    std::cout << "Robot mode was changed to moon maze.\n";
}
inline void plantTheFlag_callback(std::string_view argString)
{
    auto args = get_regex(argString);

    std::cout << "Robot mode was changed to plant the flag.\n";
}
} // namespace robotica