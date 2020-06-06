#include <world/controller.hpp>
#include <vision/cascade_classifier.hpp>


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
    }


    bool controller::update(void) {
        auto& robot  = robot::instance();
        auto& window = main_window::instance();
        auto& model  = world_model::instance();

        bool exit = robot.update();
        model.update();
        window.update();

        return exit;
    }
}