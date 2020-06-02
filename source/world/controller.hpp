#pragma once

#include <world/robot.hpp>
#include <vision/world_model.hpp>
#include <window/main_window.hpp>


namespace robotica {
    class controller {
    public:
        constexpr static int default_timestep = 1000 / 30;  // 30 TPS

        static controller& instance(void);

        controller(int timestep);

        bool update(void);

        int get_timestep(void) const { return timestep; }
    private:
        int timestep;
    };
}