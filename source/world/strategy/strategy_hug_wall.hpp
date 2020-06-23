#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>

#include <glm/glm.hpp>


namespace robotica {
    class strategy_hug_wall : public icompletable_strategy {
    public:
        enum wall_side { LEFT, RIGHT };

        strategy_hug_wall(wall_side side, maze_vertex::direction dir) : side(side), dir(dir) {
            
        }


        void init(void) override {
            
        }


        void exit(void) override {
            
        }


        void loop(void) override {
            auto& settings = main_window::instance();

            if (s == CLOSING) {
                if (side_dist(side) > 2.6) {
                    settings.speed = 10;

                    if (side == LEFT) {
                        settings.left_motor = 0.98;
                        settings.right_motor = 1;
                    } else {
                        settings.left_motor = 1;
                        settings.right_motor = 0.98;
                    }

                    delta += settings.speed * 0.98;
                } else {
                    s = REALIGING;
                }
            } else {
                
            }
        }


        bool done(void) override {
            return false;
        }
    private:
        enum state { CLOSING, REALIGING } s = CLOSING;
        bool is_done = false;
        float delta = 0;

        wall_side side;
        maze_vertex::direction dir;


        static float side_dist(wall_side side) {
            const auto& pc = world_model::instance().get_lidar_pointcloud();

            std::vector<glm::vec3> points;
            for (const auto& e : pc) {
                auto pos = e.position;

                if (std::abs(pos.x) < 2 || std::abs(pos.z) > 5 || pos.y > 1) continue;
                if (side == LEFT  && pos.x < 0) continue;
                if (side == RIGHT && pos.x > 0) continue;

                points.push_back(pos);
            }

            if (points.size() == 0) return std::numeric_limits<float>::infinity();

            double sum = 0;
            for (const auto& pt : points) sum += std::clamp(std::abs(pt.x), 0.0f, 100.0f);

            return sum / points.size();
        }


        static double target_heading(maze_vertex::direction dir) {
            if (dir == maze_vertex::NORTH) return 0;
            if (dir == maze_vertex::SOUTH) return 180;
            if (dir == maze_vertex::EAST)  return 90;
            if (dir == maze_vertex::WEST)  return 270;
        }
    };
}