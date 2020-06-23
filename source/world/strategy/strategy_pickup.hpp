#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>

#include <glm/glm.hpp>


namespace robotica {
    class strategy_pickup : public icompletable_strategy {
    public:
        strategy_pickup(std::string_view name) : name(name) {
            heading = robot::instance().get_bearing_in_degrees();
            last_rot = heading;

            std::cout << "Looking for " << name << std::endl;
        }


        void loop(void) override {
            auto distance = [](double rot, double axis) {
                auto difference = [](double a, double b) { return std::abs(a - b); };

                return std::min(
                    difference(rot, axis),
                    std::min(difference(rot - 360, axis), difference(rot + 360, axis))
                );
            };


            auto& settings = main_window::instance();
            if (s == ROTATING) {
                settings.speed = 10;
                settings.left_motor = 1;
                settings.right_motor = -1;

                if (!realign) {
                    auto objs = world_model::instance().get_raw_object_list();

                    for (const auto& o : objs) {
                        if (o.bounding_rect.x + (o.bounding_rect.size().width / 2) < robot::instance().get_camera_size(side::LEFT).x) {
                            std::cout << "Found " << name << std::endl;

                            s = FWD;
                            return;
                        }
                    }
                }

                dist_rot += difference(last_rot, robot::instance().get_bearing_in_degrees());
                last_rot = robot::instance().get_bearing_in_degrees();
                if (dist_rot >= 355) realign = true;

                if (realign && difference(heading, robot::instance().get_bearing_in_degrees()) < 1) {
                    std::cout << "Finished looking for " << name << std::endl;

                    s = DONE;

                    settings.speed = 0;
                    settings.left_motor = 0;
                    settings.right_motor = 0;
                }
            } else if (s == FWD) {
                settings.speed = 10;
                settings.left_motor = 1;
                settings.right_motor = 1;

                dist_move += settings.speed * settings.left_motor;

                auto objs = world_model::instance().get_raw_object_list();
                if (objs.empty()) {
                    s = BACK;
                }
            } else if (s == BACK) {
                settings.speed = 10;
                settings.left_motor = -1;
                settings.right_motor = -1;

                dist_move -= settings.speed * settings.left_motor;

                if (dist_move <= 0) {
                    realign = true;
                    s = ROTATING;
                }
            }
        }


        bool done(void) {
            return s == DONE;
        }
    private:
        enum state { ROTATING, FWD, BACK, DONE } s = ROTATING;

        std::string name;
        double heading, dist_move = 0, dist_rot = 0, last_rot;
        bool realign = false;

    };
}