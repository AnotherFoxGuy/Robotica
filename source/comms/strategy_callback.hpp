#pragma once


#include "world/strategy/strategy_dance.hpp"
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
    if (argString == "moondance")
        controller.set_strategy(std::make_unique<strategy_dance>("kaas.wav"));

    std::cout << "Selected robot mode has been started.\n";
}
inline void stop_callback(std::string_view argstring)
{
    auto& controller = controller::instance();

    controller.set_strategy(std::make_unique<istrategy>());

    std::cout << "Selected robot mode has been stopped.\n";
}
} // namespace robotica