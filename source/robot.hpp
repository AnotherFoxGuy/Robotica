#pragma once

#include <utility/typedefs.hpp>

#include <webots/Robot.hpp>
#include <webots/Camera.hpp>
#include <webots/Motor.hpp>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

#include <string_view>
#include <iostream>


namespace robotica {
    class robot {
    public:
        constexpr static float max_speed = 6.25f;

        const static inline std::string camera_name    = "camera";
        const static inline std::string motor_names[2] = { "left wheel motor", "right wheel motor" };


        robot(int timestep) :
            rbt(new webots::Robot()),
            camera(rbt->getCamera(camera_name)),
            left_motor(rbt->getMotor(motor_names[0])),
            right_motor(rbt->getMotor(motor_names[1])),
            timestep(timestep),
            eye_distance(0.0f),
            eye_height(0.028f)
        {
            
            camera->enable(timestep);

            left_motor->setPosition(INFINITY);
            right_motor->setPosition(INFINITY);

            left_motor->setVelocity(0);
            right_motor->setVelocity(0);
        }


        // Simulate the robot for the period dt.
        // Returns false if the simulation has ended, or true otherwise.
        bool update(void) {
            int result;
            
            if (result = rbt->step(timestep); result != -1) {
                // Go in circles.
                (*left_motor).setVelocity(max_speed);
                (*right_motor).setVelocity(0.75 * max_speed);
            }

            return (result != -1);
        }


        // Get the size of the camera viewport as { width, height }.
        glm::ivec2 get_camera_viewport_size(void) const {
            return {
                camera->getWidth(),
                camera->getHeight()
            };
        }


        // Get the raw output image of the camera. (BGR format)
        cv::Mat get_camera_output(void) const {
            const auto viewport = get_camera_viewport_size();

            cv::Mat result(viewport.y, viewport.x, CV_8UC3);

            // The documentation for Webots about the internal format of the camera data makes the following claims:
            // - The image is stored in RGB
            // - The image is stored in BGRA
            // - The image data is in array-of-structs format and not structs-of-arrays format.
            // Some of these claims contradict each other and none of them appear to be correct.
            // Fortunately, we can bypass this mess by just iterating over each pixel and using the built in functions
            // to extract image data for each channel.
            unsigned index = 0;
            for (int x = 0; x < viewport.x; ++x) {
                for (int y = 0; y < viewport.y; ++y) {
                    result.data[index + 0] = camera->imageGetBlue (camera->getImage(), viewport.x, x, y);
                    result.data[index + 1] = camera->imageGetGreen(camera->getImage(), viewport.x, x, y);
                    result.data[index + 2] = camera->imageGetRed  (camera->getImage(), viewport.x, x, y);

                    index += 3;
                }
            }

            cv::rotate(result, result, cv::ROTATE_90_CLOCKWISE);

            return result;
        }
    private:
        unique<webots::Robot> rbt;
        unique<webots::Camera> camera;
        unique<webots::Motor> left_motor, right_motor;

        int timestep;
        float eye_distance, eye_height;
    };
}