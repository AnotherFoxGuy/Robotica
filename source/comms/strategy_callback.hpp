#pragma once

#include <world/controller.hpp>
#include <world/robot.hpp>
#include <world/strategy/istrategy.hpp>
#include <world/strategy/strategy_playingcards.hpp>

#include <iostream>
#include <string>

namespace robotica
{
inline void start_callback(std::string_view argString)
{
//    std::cout << "1 " << argString << '\n';
//    const static std::regex rgx { R"REGEX((-?\d+))REGEX" };
//    auto args = regex_groups(std::string(argString), rgx);
//    std::cout << "2 " << args[0] << '\n';
    auto& controller = controller::instance();

    if (argString == "manual")
        controller.set_strategy(std::make_unique<istrategy>());
    if (argString == "hearts")
        controller.set_strategy(std::make_unique<strategy_playingcards>("Hearts"));
    if (argString == "diamonds")
        controller.set_strategy(std::make_unique<strategy_playingcards>("Diamonds"));
    if (argString == "spades")
        controller.set_strategy(std::make_unique<strategy_playingcards>("Spades"));
    if (argString == "clubs")
        controller.set_strategy(std::make_unique<strategy_playingcards>("Clubs"));

    std::cout << "Selected robot mode has been started.\n";
}
inline void stop_callback(std::string_view argstring)
{
    auto& controller = controller::instance();

    controller.set_strategy(std::make_unique<istrategy>());

    std::cout << "Selected robot mode has been stopped.\n";
}
} // namespace robotica