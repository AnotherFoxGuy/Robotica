#pragma once

#include <utility/math.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cmath>
#include <algorithm>


namespace robotica {
    class camera {
    public:
        constexpr static inline glm::vec3 absolute_up    { 0, 1, 0 };
        constexpr static inline glm::vec3 absolute_fwd   { 0, 0, 1 };
        constexpr static inline glm::vec3 absolute_right { 1, 0, 0 };


        camera(float aspect_ratio = 16.0 / 9.0) : aspect_ratio(aspect_ratio) {}


        void move(glm::vec3 delta) {
            movement += delta;
        }

        void move_to(glm::vec3 pos) {
            position = pos;
        }


        void add_pitch(float amount) {
            constexpr float half_pi = pi / 2.0;

            pitch += amount;
            pitch = std::clamp<float>(pitch, -0.999 * half_pi, 0.999 * half_pi);
        }

        void add_yaw(float amount) {
            yaw += amount;

            while (yaw >  2 * pi) yaw -= 2 * pi;
            while (yaw < -2 * pi) yaw += 2 * pi;
        }


        // Yes, I know GLM has methods for this, but I can't be bothered to find out what they're called.
        glm::mat4 get_matrix(void) {
            glm::mat4 projection = glm::perspective(
                glm::radians(90.0f),
                aspect_ratio,
                0.1f,
                1000.0f
            );


            glm::vec3 forwards = glm::normalize(glm::vec3 {
                std::cos(pitch) * std::sin(yaw),
                std::sin(pitch),
                std::cos(pitch) * std::cos(yaw)
            });

            glm::vec3 right = glm::normalize(glm::cross(absolute_up, forwards));
            glm::vec3 up    = glm::normalize(glm::cross(forwards, right));

            position += movement.x * right;
            position += movement.y * up;
            position += movement.z * forwards;
            movement = glm::vec3 { 0, 0, 0 };


            return projection * glm::lookAt(position, position + forwards, up);
        }
    private:
        glm::vec3 position, movement;
        float pitch, yaw;
        float aspect_ratio;
    };
}