#pragma once

#include <utility/typedefs.hpp>
#include <vision/converters.hpp>
#include <window/main_window.hpp>

#include <webots/Robot.hpp>
#include <webots/Camera.hpp>
#include <webots/Motor.hpp>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

#include <string_view>
#include <iostream>


namespace robotica {
    enum class side { LEFT = 0, RIGHT = 1 };
    constexpr inline auto sides = { robotica::side::LEFT, robotica::side::RIGHT };


    class robot {
    public:
        constexpr static int default_timestep = 1000 / 30;

        static robot& instance(void);


        constexpr static float max_speed = 6.25f;

        const static inline std::string camera_names[2] = { "left_camera", "right_camera" };
        const static inline std::string motor_names[2]  = { "left wheel motor", "right wheel motor" };


        robot(int timestep);


        // Updates the simulation. Returns false if the simulation has ended.
        bool update(void);

        // Camera output is a reference to the WeBots buffer and will be overridden on robot update!
        cv::Mat    get_camera_output(side side) const;
        float      get_camera_fov   (side side) const;
        glm::ivec2 get_camera_size  (side side) const;
    private:
        unique<webots::Robot> rbt;
        unique<webots::Camera> left_camera, right_camera;
        unique<webots::Motor> left_motor, right_motor;

        int timestep;
        float eye_distance, eye_height;
    };
}