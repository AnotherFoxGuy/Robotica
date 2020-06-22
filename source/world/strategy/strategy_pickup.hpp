#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>

#include <glm/glm.hpp>


namespace robotica {
    class strategy_pickup : public icompletable_strategy {
    public:
        bool has_object = false;

        void init(void) override {
            last_heading = robot::instance().get_bearing_in_degrees();
            rotated = 0;
        }


        void exit(void) override {
            auto& settings = main_window::instance();
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            if (done()) return;

            auto& rbt = robot::instance();
            auto& settings = main_window::instance();


            if (rotated >= 360) {
                is_done = true;

                settings.left_motor = 0;
                settings.right_motor = 0;

                return;
            }

            if (s == ROTATING) {
                double heading = rbt.get_bearing_in_degrees();
                apply_rotation(heading);

                settings.speed = 10;
                settings.left_motor = 1;
                settings.right_motor = -1;


                if (!has_object) {
                    const auto& objs = world_model::instance().get_raw_object_list();
                    if (auto it = std::find_if(objs.begin(), objs.end(), [](const classified_object& o) { return o.type == "Rock"; }); it != objs.end()) {
                        auto obj = *it;

                        if (obj.bounding_rect.x + (obj.bounding_rect.size().width / 2) < rbt.get_camera_size(side::LEFT).x / 2) s = MOVING_FWD;
                    }
                }
            } else if (s == MOVING_FWD) {
                settings.speed = 25;
                settings.left_motor = 1;
                settings.right_motor = 1;

                ++travelled;

                const auto& objs = world_model::instance().get_object_list();
                if (std::find_if(objs.begin(), objs.end(), [](const world_model::world_object& o) { return o.identity == "Rock"; }) == objs.end()) s = GRABBING;
            } else if (s == MOVING_BACK) {
                if (travelled == 0) s = ROTATING;
                else --travelled;

                settings.speed = 25;
                settings.left_motor = -1;
                settings.right_motor = -1;
            } else if (s == GRABBING) {
                // Gripper code goes here.

                has_object = true;
                s = MOVING_BACK;
            }
        }


        bool done(void) override {
            return is_done;
        }
    private:
        enum state { ROTATING, MOVING_FWD, MOVING_BACK, GRABBING } s = ROTATING;


        bool is_done = false;
        double rotated, last_heading;
        int travelled = 0;


        static bool is_same(const glm::vec2& a, const glm::vec2& b) {
            return false;
        }


        void apply_rotation(double heading) {
            double tmp = (heading > last_heading) ? heading - 360 : heading;
            rotated += std::abs(heading - last_heading);

            last_heading = heading;
        }
    };
}