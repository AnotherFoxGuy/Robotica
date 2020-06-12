#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cmath>


namespace robotica {
    class camera {
    public:
        constexpr static inline glm::vec3 absolute_up    { 0, 1, 0 };
        constexpr static inline glm::vec3 absolute_fwd   { 0, 0, 1 };
        constexpr static inline glm::vec3 absolute_right { 1, 0, 0 };


        void move(glm::vec3 delta) {
            movement += delta;
        }

        void add_pitch(float amount) {
            pitch += amount;
        }

        void add_yaw(float amount) {
            yaw += amount;
        }


        // Yes, I know GLM has methods for this, but I can't be bothered to find out what they're called.
        glm::mat4 get_matrix(void) {
            glm::vec3 camera_up    = glm::normalize(rotate_vec(absolute_up, absolute_right, pitch));
            glm::vec3 camera_fwd   = glm::normalize(rotate_vec(rotate_vec(absolute_fwd, absolute_right, pitch), camera_up, yaw));
            glm::vec3 camera_right = glm::normalize(glm::cross(camera_fwd, camera_up));

            position += movement.x * camera_right;
            position += movement.y * camera_up;
            position += movement.z * camera_fwd;
            movement = glm::vec3{};

            return glm::mat4 {
                camera_right.x,     camera_right.y,     camera_right.z,     0,
                camera_up.x,        camera_up.y,        camera_up.z,        0,
                camera_fwd.x,       camera_fwd.y,       camera_fwd.z,       0,
                0,                  0,                  0,                  1
            } * glm::mat4 {
                1,                  0,                  0,                  -position.x,
                0,                  1,                  0,                  -position.y,
                0,                  0,                  1,                  -position.z,
                0,                  0,                  0,                  1
            };
        }
    private:
        glm::vec3 position, movement;
        float pitch, yaw;

        static glm::vec3 rotate_vec(glm::vec3 vec, glm::vec3 axis, float angle) {
            return std::cos(angle) * vec + std::sin(angle) * glm::cross(axis, vec) + (1 - std::cos(angle)) * glm::dot(axis, vec) * axis;
        }
    };
}