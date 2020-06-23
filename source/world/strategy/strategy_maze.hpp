#pragma once

#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/istrategy.hpp>
#include <world/strategy/strategy_pickup.hpp>
#include <utility/typedefs.hpp>
#include <vision/pool_classifier.hpp>
#include <vision/rock_classifier.hpp>
#include <world/strategy/mazemap.hpp>
#include <world/strategy/stategy_goto.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace robotica {
    // TODO: Replace all this init/exit bs with RAII.
    class strategy_maze : public istrategy {
    public:
        strategy_maze(void) {
            world_model::instance().add_classifier(std::make_unique<pool_classifier>());
            world_model::instance().add_classifier(std::make_unique<rock_classifier>());
        }


        void init(void) override {
            todo = { 10, 9, 12, 4 };
            main_window::instance().speed = 0;
        }

        void exit(void) override {
            if (mover) mover->exit();
            if (observer) observer->exit();
        }

        void loop(void) override {
            if (done) return;

            if (s == MOVING) {
                if (mover == nullptr) {
                    mover = std::make_unique<strategy_goto>(&map, current, todo.back());
                    mover->init();
                }

                mover->loop();
                if (mover->done()) {
                    std::cout << "Destination reached. Looking for objects...\n";

                    mover->exit();

                    s = POOLS;

                    mover = nullptr;

                    current = todo.back();
                    todo.pop_back();
                }
            } else if (s == POOLS) {
                if (observer == nullptr) {
                    observer = std::make_unique<strategy_pickup>("Pool");
                    observer->init();
                }

                observer->loop();
                if (observer->done()) {
                    observer->exit();

                    s = ROCKS;
                    observer = nullptr;
                }
            } else if (s == ROCKS) {
                if (observer == nullptr) {
                    observer = std::make_unique<strategy_pickup>("Rock");
                    observer->init();
                }

                observer->loop();
                if (observer->done()) {
                    observer->exit();

                    s = MOVING;
                    observer = nullptr;
                }
            }
        }

    private:
        enum state { MOVING, POOLS, ROCKS } s;

        unique<strategy_goto> mover;
        unique<strategy_pickup> observer;

        std::vector<int> todo;
        int current = 0;

        mazemap map = generate_map();
        bool done = false;
    };
}