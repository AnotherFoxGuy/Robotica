#pragma once

#include <window/main_window.hpp>
#include <vision/world_model.hpp>
#include <world/strategy/istrategy.hpp>
#include <world/strategy/strategy_pickup.hpp>
#include <utility/typedefs.hpp>
#include <vision/pool_classifier.hpp>
#include <vision/rock_classifier.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace robotica {
    struct room {
        std::vector<shared<room>> connections;
        int rock_count;
        bool scanned;
    };

    struct maze {
        std::vector<shared<room>> rooms;
        shared<room> current;
    };


    class strategy_maze : public istrategy {
    public:
        void init(void) override {
            world_model::instance().add_classifier(std::make_unique<pool_classifier>());
            world_model::instance().add_classifier(std::make_unique<rock_classifier>());

            substrategy = std::make_unique<strategy_pickup>();
            substrategy->init();
        }

        void exit(void) override {
            substrategy->exit();
        }

        void loop(void) override {
            substrategy->loop();
        }

    private:
        unique<istrategy> substrategy;

        bool has_rock = false;
    };
}