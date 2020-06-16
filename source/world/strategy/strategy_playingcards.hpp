#pragma once

#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/istrategy.hpp>

#include <cstddef>
#include <string>

namespace robotica {
class strategy_playingcards : public istrategy {
  public:
    strategy_playingcards(std::string card) {
        playingcard = card;
    }

    void init(void) override {
        main_window::instance().speed = 10;
        main_window::instance().min_neighbours = 10;
    }

    void exit(void) override {
        main_window::instance().speed = 1;
        main_window::instance().min_neighbours = 64;
    }

    void loop(void) override {
        auto& objects = world_model::instance().get_object_list();
        if (auto it = std::find_if(objects.begin(), objects.end(), [&](const auto& o) { return o.identity == playingcard;}); it != objects.end() && !resetting) {
            main_window::instance().left_motor = 1;
            main_window::instance().right_motor = 1;

            lookingForTarget = false;
            ++ticks;
        } else if(lookingForTarget) {
            main_window::instance().left_motor = -1;
            main_window::instance().right_motor = 1;
        } else if (ticks > 0) {
            main_window::instance().left_motor = -1;
            main_window::instance().right_motor = -1;

            resetting = true;
            --ticks;
        } else {
            main_window::instance().left_motor = 0;
            main_window::instance().right_motor = 0;
        }
    }

  private:
    std::string playingcard;
    std::size_t ticks = 0;
    bool resetting = false;
    bool lookingForTarget = true;
};
} // namespace robotica