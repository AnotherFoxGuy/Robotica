#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>

#include <glm/glm.hpp>

using namespace std::string_literals;


namespace robotica {
    class strategy_hug_wall : public icompletable_strategy {
    public:
        constexpr static float d_pwr = 0.45;
        constexpr static float d_max = 300;

        float distance_travelled = 0;

        strategy_hug_wall(side sd) : sd(sd) {
            std::cout << "Hugging " << ((sd == side::LEFT) ? "left"s : "right"s) << " wall." << std::endl;
        }


        void init(void) override {
            target_facing = robot::instance().get_bearing_in_degrees();
            start_dist = side_dist(sd);
        }


        void exit(void) override {
            auto& settings = main_window::instance();

            settings.speed = 0;
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            auto& settings = main_window::instance();


            auto distance = [](double rot, double axis) {
                auto difference = [](double a, double b) { return std::abs(a - b); };

                return std::min(
                    difference(rot, axis),
                    std::min(difference(rot - 360, axis), difference(rot + 360, axis))
                );
            };


            if (s == CLOSING) {
                if (side_dist(sd) > 0.15 && (side_dist(sd) < start_dist || ticks++ < 30)) {
                    settings.speed = 10;

                    if (delta < d_max) {
                        if (sd == side::LEFT) {
                            settings.left_motor = d_pwr;
                            settings.right_motor = 1;
                        } else {
                            settings.left_motor = 1;
                            settings.right_motor = d_pwr;
                        }

                        delta += settings.speed * d_pwr;
                        distance_travelled = delta;
                    } else {
                        settings.left_motor = 1;
                        settings.right_motor = 1;

                        distance_travelled += settings.speed * settings.left_motor;
                    }
                } else {
                    std::cout << "Realigning to path" << std::endl;
                    s = REALIGING;
                }
            } else if (s == REALIGING) {
                if (difference(robot::instance().get_bearing_in_degrees(), target_facing) > 0.1) {
                    settings.speed = 10;

                    if (sd == side::LEFT) {
                        settings.left_motor = 1;
                        settings.right_motor = d_pwr;
                    } else {
                        settings.left_motor = d_pwr;
                        settings.right_motor = 1;
                    }
                } else {
                    std::cout << "Finished path realignment" << std::endl;
                    s = DONE;
                }
            }
        }


        bool done(void) override {
            return s == DONE;
        }
    private:
        enum state { CLOSING, REALIGING, DONE } s = CLOSING;
        bool is_done = false;
        float delta = 0;
        float target_facing;
        float start_dist;
        int ticks = 0;

        side sd;


        static float side_dist(side sd) {
            return robot::instance().get_distance(sd);
        }
    };
}