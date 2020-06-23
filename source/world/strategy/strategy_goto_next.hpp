#pragma once

#include <world/strategy/icompletable_strategy.hpp>
#include <world/robot.hpp>
#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/mazemap.hpp>
#include <world/strategy/strategy_hug_wall.hpp>
#include <world/strategy/strategy_lookat.hpp>

#include <glm/glm.hpp>


namespace robotica {
    class strategy_goto_next : public icompletable_strategy {
    public:
        strategy_goto_next(float distance, maze_vertex::direction current, maze_vertex::direction next) : distance(distance), current(current), next(next) {}


        void exit(void) override {
            auto& settings = main_window::instance();

            settings.speed = 0;
            settings.left_motor = 0;
            settings.right_motor = 0;
        }


        void loop(void) override {
            if (done()) return;

            if (!did_turn) {
                // Turn at the start of each transfer to face the correct direction.
                if (rotator == nullptr) {
                    rotator = std::make_unique<strategy_lookat>(current);
                    rotator->init();
                }

                rotator->loop();
                if (rotator->done()) {
                    rotator->exit();

                    did_turn = true;
                }
            } else if (auto side = needs_align(); started_align || (!did_align && side && can_align(side.value()))) {
                started_align = true;

                // Align to the wall so we can turn more easily.
                if (aligner == nullptr) {
                    aligner = std::make_unique<strategy_hug_wall>(side.value());
                    aligner->init();
                }

                aligner->loop();
                if (aligner->done()) {
                    aligner->exit();

                    did_align = true;
                    started_align = false;
                    travelled += aligner->distance_travelled;
                }
            } else {
                // Move forwards.
                auto& settings = main_window::instance();

                settings.speed = 25;
                settings.left_motor = 1;
                settings.right_motor = 1;
                travelled += settings.speed * settings.left_motor;

                if (!reached_target && detect_vert_reached(get_turn_side())) {
                    std::cout << "Detected next vertex. Finishing current movement." << std::endl;
                    travelled = std::max(travelled, distance - 400);
                    reached_target = true;
                }
            }
        }


        bool done(void) override {
            return travelled >= distance;
        }
    private:
        bool did_align = false, started_align = false, did_turn = false, reached_target = false;
        unique<strategy_hug_wall> aligner;
        unique<strategy_lookat> rotator;
        float distance = 0, travelled = 0;
        maze_vertex::direction current, next;


        bool detect_vert_reached(side s) {
            if (!did_align) return false;
            return robot::instance().get_distance(s) >= 1;
        }


        bool can_align(side s) {
            auto distance = robot::instance().get_distance(s);
            return distance < 1 && distance > 0.15;
        }


        std::optional<side> needs_align(void) {
            if (current == next || current == inverse(next)) return std::nullopt;
            else return get_turn_side();
        }


        side get_turn_side(void) {
            if (current == maze_vertex::NORTH) return (next == maze_vertex::EAST)  ? side::LEFT  : side::RIGHT;
            if (current == maze_vertex::SOUTH) return (next == maze_vertex::EAST)  ? side::RIGHT : side::LEFT;
            if (current == maze_vertex::EAST ) return (next == maze_vertex::NORTH) ? side::RIGHT : side::LEFT;
            if (current == maze_vertex::WEST ) return (next == maze_vertex::NORTH) ? side::LEFT  : side::RIGHT;
        }
    };
}