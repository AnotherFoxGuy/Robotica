#pragma once

#include <utility/typedefs.hpp>
#include <vision/converters.hpp>
#include <window/main_window.hpp>

#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <webots/Camera.hpp>
#include <webots/Compass.hpp>
#include <webots/Lidar.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>

#include <iostream>
#include <string_view>


namespace robotica {
    enum class side { LEFT = 0, RIGHT = 1 };
    constexpr inline auto sides = { robotica::side::LEFT, robotica::side::RIGHT };


    class robot {
    public:
        constexpr static int default_timestep = 1000 / 30;

        static robot& instance(void);


        constexpr static float max_speed = 6.25f;

        const static inline std::string camera_names[2] = { "left_camera", "right_camera" };
        //const static inline std::string motor_names[2] = { "left wheel motor", "right wheel motor" };
        const static inline std::string motor_names[5] = { "aandrijf_links", "aandrijf_rechts", "basis_kraan", "korte_arm", "lange_arm" };

        const static inline std::string compass_name = "compass";
        const static inline std::string lidar_name   = "lidar_sensor";

        robot(int timestep);
        ~robot(void);


        // Updates the simulation. Returns false if the simulation has ended.
        bool update(void);
        double get_bearing_in_degrees();

        // Camera output is a reference to the WeBots buffer and will be overridden on robot update!
        cv::Mat    get_camera_output      (side side) const;
        float      get_camera_fov         (side side) const;
        glm::ivec2 get_camera_size        (side side) const;
        float      get_camera_focal_length(side side) const;
        float      get_camera_baseline    (void)      const;
    private:
        webots::Robot* rbt;
        webots::Camera *left_camera, *right_camera;
        webots::Motor  *left_motor, *right_motor, *arm_base, *arm_short, *arm_long;
        webots::Compass* compass;
        webots::Lidar* lidar;

        int timestep;
        float eye_distance, eye_height;
        bool manually_destroyed = false;
    };
}
