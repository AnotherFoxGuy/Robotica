#include <world/controller.hpp>


namespace robotica {
    controller& controller::instance(void) {
        static controller i { default_timestep };
        return i;
    }


    controller::controller(int timestep) : timestep(timestep) {
        world_model::instance().add_classifier("ROCK", "moonrock.xml");
        world_model::instance().add_classifier("Heart", "Hearts.xml");
        world_model::instance().add_classifier("Diamond", "Diamonds.xml");
        world_model::instance().add_classifier("Spade", "Spades.xml");
        world_model::instance().add_classifier("Club", "Clubs.xml");
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