#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>
#include <world/strategy/strategy_lookat.hpp>
#include <world/strategy/strategy_hug_wall.hpp>

#include <glm/glm.hpp>

#include <queue>
#include <deque>
#include <vector>
#include <unordered_set>
#include <unordered_map>


namespace robotica {
    class strategy_goto : public icompletable_strategy {
    public:
        strategy_goto(const mazemap* map, int current, int dest) : map(map) {
            todo = get_path(map->vertices[current], map->vertices[dest], *map);
        }


        void exit(void) override {
            auto& settings = main_window::instance();
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            if (todo.empty()) return;
            auto& settings = main_window::instance();

            if (s == ROTATING) {
                if (rotate == nullptr) {
                    rotate = std::make_unique<strategy_lookat>(todo[0].ab);
                    rotate->init();
                }

                rotate->loop();
                if (rotate->done()) {
                    s = ALIGNING;

                    rotate->exit();
                    rotate = nullptr;
                }
            } else if (s == MOVING) {
                if (distance < todo[0].distance) {
                    settings.speed = 25;
                    settings.left_motor = 1;
                    settings.right_motor = 1;

                    distance += settings.speed * settings.left_motor;
                } else {
                    settings.speed = 0;
                    settings.left_motor = 0;
                    settings.right_motor = 0;

                    todo.pop_front();
                    distance = 0;

                    s = ROTATING;
                }
            } else if (s == ALIGNING) {
                if (todo.size() < 2 || todo[0].ab == todo[1].ab) {
                    s = MOVING;
                    return;
                }

                if (rotate == nullptr) {
                    rotate = std::make_unique<strategy_hug_wall>(side_for_dirchange(todo[0].ab, todo[1].ab), todo[0].ab);
                    rotate->init();
                }

                rotate->loop();
                if (rotate->done()) {
                    rotate = nullptr;
                    s = MOVING;
                }
            }
        }


        bool done(void) override {
            return todo.empty();
        }
    private:
        enum state { MOVING, ALIGNING, ROTATING } s = ROTATING;

        const mazemap* map;
        std::deque<maze_vertex::edge> todo;
        unique<icompletable_strategy> rotate;
        float distance = 0;


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


        static strategy_hug_wall::wall_side side_for_dirchange(maze_vertex::direction from, maze_vertex::direction to) {
            if (from == maze_vertex::EAST)  return (to == maze_vertex::NORTH) ? strategy_hug_wall::RIGHT : strategy_hug_wall::LEFT;
            if (from == maze_vertex::WEST)  return (to == maze_vertex::SOUTH) ? strategy_hug_wall::RIGHT : strategy_hug_wall::LEFT;
            if (from == maze_vertex::NORTH) return (to == maze_vertex::EAST)  ? strategy_hug_wall::RIGHT : strategy_hug_wall::LEFT;
            if (from == maze_vertex::SOUTH) return (to == maze_vertex::WEST)  ? strategy_hug_wall::RIGHT : strategy_hug_wall::LEFT;
        }
    };
}