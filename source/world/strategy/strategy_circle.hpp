#pragma once

#include <world/strategy/istrategy.hpp>
#include <window/main_window.hpp>

#include <cstddef>


namespace robotica {
    class strategy_circle : public istrategy {
    public:
        void init(void) override {
            main_window::instance().speed = 35;
        }


        void exit(void) override {
            main_window::instance().speed = 0;
        }


        void loop(void) override {
            if (ticks % 1000 == 0) {
                main_window::instance().left_motor = 1;
                main_window::instance().right_motor = -1;
            } else if (ticks % 500 == 0) {
                main_window::instance().left_motor = -1;
                main_window::instance().right_motor = 1;
            }

            ++ticks;
        }
    private:
        std::size_t ticks = 0;
    };
}