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

        static robot& instance(void) {
            static robot i = robot { default_timestep };
            return i;
        }


        constexpr static float max_speed = 6.25f;

        const static inline std::string camera_names[2] = { "left_camera", "right_camera" };
        const static inline std::string motor_names[2]  = { "left wheel motor", "right wheel motor" };


        robot(int timestep) :
            rbt(new webots::Robot()),
            left_camera(rbt->getCamera(camera_names[0])),
            right_camera(rbt->getCamera(camera_names[1])),
            left_motor(rbt->getMotor(motor_names[0])),
            right_motor(rbt->getMotor(motor_names[1])),
            timestep(timestep),
            eye_distance(0.03f),
            eye_height(0.028f)
        {
            left_camera->enable(timestep);
            right_camera->enable(timestep);

            left_motor->setPosition(INFINITY);
            right_motor->setPosition(INFINITY);

            left_motor->setVelocity(0);
            right_motor->setVelocity(0);
        }


        // Simulate the robot for the period dt.
        // Returns false if the simulation has ended, or true otherwise.
        bool update(void) {
            auto& window = main_window::instance();

            int result;
            
            if (result = rbt->step(timestep); result != -1) {
                // Go in circles.
                (*left_motor).setVelocity(window.left_motor * max_speed);
                (*right_motor).setVelocity(window.right_motor * max_speed);
            }

            return (result != -1);
        }

        // camera clears the buffer
        // if you want to save the data you need to make a copy of "image", otherwise it will be overwritten
        cv::Mat get_camera_output(side side) const {
            auto& camera = (side == side::LEFT) ? left_camera : right_camera;

            cv::Mat image = cv::Mat(cv::Size(camera->getWidth(), camera->getHeight()), CV_8UC4);
            image.data = (uchar *)camera->getImage();

            return image;
        }
    private:
        unique<webots::Robot> rbt;
        unique<webots::Camera> left_camera, right_camera;
        unique<webots::Motor> left_motor, right_motor;

        int timestep;
        float eye_distance, eye_height;
    };
}