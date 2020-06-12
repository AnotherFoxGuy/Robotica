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


        camera(float aspect_ratio = 16.0 / 9.0) : aspect_ratio(aspect_ratio) {}


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
            glm::vec3 direction = {
                std::cos(yaw) * std::cos(pitch),
                std::sin(pitch),
                std::sin(yaw) * cos(pitch)
            };


            glm::vec3 camera_fwd   = glm::normalize(position + direction);
            glm::vec3 camera_right = glm::normalize(glm::cross(absolute_up, camera_fwd));
            glm::vec3 camera_up    = glm::cross(camera_fwd, camera_right);


            position += movement.x * camera_right;
            position += movement.y * camera_up;
            position += movement.z * camera_fwd;
            movement = glm::vec3 { };


            return glm::infinitePerspective(90.0f, aspect_ratio, 0.001f) * glm::lookAt(position, position + direction, { 0, 1, 0 });
        }
    private:
        glm::vec3 position, movement;
        float pitch, yaw;
        float aspect_ratio;
    };
}