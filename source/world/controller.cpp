#include <world/controller.hpp>
#include <vision/world_model.hpp>
#include <vision/cascade_classifier.hpp>
#include <vision/pool_classifier.hpp>
#include <comms/websocket.hpp>
#include <comms/joystick_callback.hpp>
#include <window/main_window.hpp>


namespace robotica {
    controller& controller::instance(void) {
        static controller i { default_timestep };
        return i;
    }


    controller::controller(int timestep) : timestep(timestep) {
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "moonrock.xml", "Rock"   ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Hearts.xml",   "Heart"  ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Diamonds.xml", "Diamond"));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Spades.xml",   "Spade"  ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Clubs.xml",    "Club"   ));
        world_model::instance().add_classifier(std::make_unique<pool_classifier>());

        websocket::instance().init();
        websocket::instance().add_callback("joystick", &joystick_callback);
    }


    bool controller::update(void) {
        bool exit = robot::instance().update();
        world_model::instance().update();
        websocket::instance().update();
        main_window::instance().update();

        return exit;
    }
}