#pragma once

#include <utility/typedefs.hpp>
#include <vision/converters.hpp>
#include <window/main_window.hpp>
#include <window/3d/primitive.hpp>

#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <webots/Camera.hpp>
#include <webots/Compass.hpp>
#include <webots/Lidar.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/LED.hpp>
#include <webots/Speaker.hpp>
#include <webots/Display.hpp>
#include <webots/TouchSensor.hpp>

#include <iostream>
#include <string_view>
#include <vector>


namespace robotica {
    enum class side { LEFT = 0, RIGHT = 1 };
    constexpr inline auto sides = { robotica::side::LEFT, robotica::side::RIGHT };


    class robot {
    public:

        static robot& instance(void);

        const static inline std::string camera_names[2] = { "left_camera", "right_camera" };
        const static inline std::string motor_names[9] = {"left_motor", "right_motor", "basis_kraan", "korte_arm", "lange_arm", "grijper_links", "grijper_rechts", "tandwielkast_grijper", "arm_grijper" };

        const static inline std::string compass_name = "compass";
        const static inline std::string lidar_name = "lidar_sensor";
        const static inline std::string speaker_name = "speaker";
        const static inline std::string display_name = "display";
        const static inline std::string scale_name = "scale";
        const static inline std::string led_names[2] = {"led_left", "led_right"};

        robot(int timestep);
        ~robot(void);


        // Updates the simulation. Returns false if the simulation has ended.
        bool update(void);
        void update_emotion();
        void set_led_color(int, int);
        double get_bearing_in_radian();
        double get_bearing_in_degrees();

        // Camera output is a reference to the WeBots buffer and will be overridden on robot update!
        cv::Mat    get_camera_output      (side side) const;
        float      get_camera_fov         (side side) const;
        glm::ivec2 get_camera_size        (side side) const;
        float      get_camera_focal_length(side side) const;
        float      get_camera_baseline    (void)      const;

        webots::Speaker* speaker;
        webots::ImageRef* emotes;
        webots::Display* display;
        webots::LED *led_left, *led_right;

        pointcloud get_lidar_pointcloud(void) const;

        std::string current_emotion = "sing";
    private:
        webots::Robot* rbt;
        webots::Camera *left_camera, *right_camera;
        webots::Motor *left_motor, *right_motor, *arm_base, *arm_short, *arm_long, *gripper_left, *gripper_right, *gripper_roll, *gripper_pitch;
        webots::Compass* compass;
        webots::Lidar* lidar;
        webots::TouchSensor* scale;

        int timestep;
        int frames;
        float eye_distance, eye_height;
        bool manually_destroyed = false;

        std::string displayed_emotion = "";
    };
}
