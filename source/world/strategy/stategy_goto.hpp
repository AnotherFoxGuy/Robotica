#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>
#include <world/strategy/strategy_lookat.hpp>
#include <world/strategy/strategy_hug_wall.hpp>
#include <world/strategy/strategy_goto_next.hpp>

#include <glm/glm.hpp>

#include <queue>
#include <deque>
#include <vector>
#include <unordered_set>
#include <unordered_map>


namespace robotica {
    class strategy_goto : public icompletable_strategy {
    public:
        strategy_goto(const mazemap* map, int from, int to) : map(map), path(get_path(map->vertices[from], map->vertices[to], *map)) {}


        void exit(void) override {
            auto& settings = main_window::instance();

            settings.speed = 0;
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            if (done()) return;

            const maze_vertex::edge& current = path.front();
            if (mover == nullptr) {
                std::cout << "Now moving to node " << current.dest << "\n";

                auto next = path.size() >= 2 ? path[1].ab : current.ab;
                mover = std::make_unique<strategy_goto_next>(current.distance, current.ab, next);
                mover->init();
            }

            mover->loop();
            if (mover->done()) {
                mover->exit();
                mover = nullptr;

                path.pop_front();
            }
        }


        bool done(void) override {
            return path.empty();
        }
    private:
        const mazemap* map;
        std::deque<maze_vertex::edge> path;
        unique<strategy_goto_next> mover;

        static std::deque<maze_vertex::edge> get_path(const maze_vertex& a, const maze_vertex& b, const mazemap& map) {
            std::queue<int> todo;
            todo.push(a.id);

            std::unordered_map<int, maze_vertex::edge> retrace;

            while (!todo.empty()) {
                int current = todo.front();
                todo.pop();

                for (const auto& edge : map.vertices[current].edges) {
                    if (retrace.contains(edge.dest)) continue;

                    todo.push(edge.dest);
                    retrace.insert(std::pair{ edge.dest, edge });

                    if (edge.dest == b.id) break;
                }
            }

            std::deque<maze_vertex::edge> result { retrace[b.id] };
            while (result.back().src != a.id) result.push_back(retrace[result.back().src]);
            std::reverse(result.begin(), result.end());

            return result;
        }


        static maze_vertex::direction get_facing(void) {
            auto& rbt = robot::instance();

            std::array facings {
                std::pair { maze_vertex::NORTH, 0.0   },
                std::pair { maze_vertex::SOUTH, 180.0 },
                std::pair { maze_vertex::EAST,  90.0  },
                std::pair { maze_vertex::WEST,  270.0 }
            };

            auto distance = [](double rot, double axis) {
                auto difference = [](double a, double b) { return std::abs(a - b); };

                return std::min(
                    difference(rot, axis),
                    std::min(difference(rot - 360, axis), difference(rot + 360, axis))
                );
            };

            auto e = std::min_element(facings.begin(), facings.end(), [&](const auto& a, const auto& b) { return distance(rbt.get_bearing_in_degrees(), a.second) < distance(rbt.get_bearing_in_degrees(), b.second); });
            return e->first;
        }
    };
}