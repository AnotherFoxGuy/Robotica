#include <world/controller.hpp>
#include <world/strategy/strategy_circle.hpp>
#include <vision/world_model.hpp>
#include <vision/cascade_classifier.hpp>
#include <vision/pool_classifier.hpp>
#include <comms/websocket.hpp>
#include <comms/joystick_callback.hpp>
#include <comms/gripper_callback.hpp>
#include <window/main_window.hpp>


namespace robotica {
    controller& controller::instance(void) {
        static controller i { default_timestep };
        return i;
    }

    controller::controller(int timestep) : timestep(timestep) {
        //world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "moonrock.xml", "Rock"   ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Hearts.xml",   "Heart"  ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Diamonds.xml", "Diamond"));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Spades.xml",   "Spade"  ));
        world_model::instance().add_classifier(std::make_unique<cascade_classifier>( "Clubs.xml",    "Club"   ));
        //world_model::instance().add_classifier(std::make_unique<pool_classifier>());

        websocket::instance().init();
        websocket::instance().add_callback("speed", &speed_callback);
        websocket::instance().add_callback("joystick", &joystick_callback);
        websocket::instance().add_callback("rotate_arm_base", &arm_base_callback);
        websocket::instance().add_callback("arm_short", &arm_short_callback);
        websocket::instance().add_callback("arm_long", &arm_long_callback);
        websocket::instance().add_callback("gripper", &gripper_callback);
        websocket::instance().add_callback("gripper_pitch", &gripper_pitch_callback);
        websocket::instance().add_callback("gripper_roll", &gripper_roll_callback);

        //set_strategy(std::make_unique<strategy_circle>());
    }

    bool controller::update(void) {
        bool not_done = robot::instance().update() && !should_exit;

        if (not_done) {
            world_model::instance().update();
            websocket::instance().update();

            //strategy->loop();

            main_window::instance().update();
        }

        return not_done;
    }
}
