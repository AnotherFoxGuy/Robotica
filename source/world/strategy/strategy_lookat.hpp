#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>

#include <glm/glm.hpp>


namespace robotica {
    class strategy_lookat : public icompletable_strategy {
    public:
        strategy_lookat(maze_vertex::direction dir) : dir(dir) {
            std::cout << robot::instance().get_bearing_in_degrees() << " - " << target_heading(dir) << std::endl;
        }


        void init(void) override {
            auto& settings = main_window::instance();
            settings.speed = 10;
            settings.left_motor = 1;
            settings.right_motor = -1;
        }


        void exit(void) override {
            auto& settings = main_window::instance();
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            auto heading = robot::instance().get_bearing_in_degrees();

            if (!approx(heading, target_heading(dir))) {
                auto& settings = main_window::instance();

                settings.speed = 10;
                settings.left_motor = 1;
                settings.right_motor = -1;
            } else {
                auto& settings = main_window::instance();
                settings.left_motor = 0;
                settings.right_motor = 0;

                is_done = true;
            }
        }


        bool done(void) override {
            return is_done;
        }
    private:
        const maze_vertex::direction dir;
        bool is_done = false;


        static double target_heading(maze_vertex::direction dir) {
            if (dir == maze_vertex::NORTH) return 0;
            if (dir == maze_vertex::SOUTH) return 180;
            if (dir == maze_vertex::EAST)  return 90;
            if (dir == maze_vertex::WEST)  return 270;
        }


        static bool approx(double a, double b) {
            return std::abs(a - b) <= 1 || std::abs((a - 360) - b) <= 1 || std::abs(a - (b - 360)) <= 1;
        }
    };
}