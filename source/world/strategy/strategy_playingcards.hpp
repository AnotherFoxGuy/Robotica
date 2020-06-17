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
        speed = main_window::instance().speed;
        neighbours = main_window::instance().min_neighbours;
    }

    void init(void) override {
        main_window::instance().speed = 50;
        main_window::instance().min_neighbours = 10;
    }

    void exit(void) override {
        main_window::instance().speed = speed;
        main_window::instance().min_neighbours = neighbours;
    }

    void loop(void) override {
        auto& objects = world_model::instance().get_raw_object_list();
        if (auto it = std::find_if(objects.begin(), objects.end(), [&](const auto& o) { return o.type == playingcard;}); it != objects.end() && !resetting) {
            auto center = it->bounding_rect.x + (it->bounding_rect.width / 2);

            if(center > 121 && center < 124 && lookingForTarget) {
                lookingForTarget = false;
            } else if (center < 121 && lookingForTarget) {
                main_window::instance().speed = 5;
                main_window::instance().left_motor = -1;
                main_window::instance().right_motor = 1;
            } else if(center > 124 && lookingForTarget) {
                main_window::instance().speed = 5;
                main_window::instance().left_motor = 1;
                main_window::instance().right_motor = -1;
            } else if (ticks > 75) {
                resetting = true;
            }else if(!lookingForTarget) {
                main_window::instance().speed = 50;
                main_window::instance().left_motor = 1;
                main_window::instance().right_motor = 1;

                ++ticks;
            }
        } else if(lookingForTarget) {
            main_window::instance().left_motor = -1;
            main_window::instance().right_motor = 1;
        } else if (ticks > 0) {
            main_window::instance().left_motor = -1;
            main_window::instance().right_motor = -1;

            --ticks;
        } else {
            main_window::instance().left_motor = 0;
            main_window::instance().right_motor = 0;
        }
    }

  private:
    std::string playingcard;
    int speed;
    int neighbours;
    std::size_t ticks = 0;
    bool resetting = false;
    bool lookingForTarget = true;
};
} // namespace robotica