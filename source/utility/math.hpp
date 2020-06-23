#pragma once

#include <glm/glm.hpp>

#include <cmath>


namespace robotica {
    constexpr inline long double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089;


    constexpr inline float degrees_to_radians(float degrees) {
        return degrees * (float(pi) / 180);
    }


    constexpr inline glm::vec2 rotate_point(const glm::vec2& point, const glm::vec2& origin, float angle) {
        return {
            std::cos(angle) * (point.x - origin.x) - std::sin(angle) * (point.y - origin.y) + origin.x,
            std::sin(angle) * (point.x - origin.x) + std::cos(angle) * (point.y - origin.y) + origin.y
        };
    }


    /// <summary>
    /// Check if a point in the world is visible from a camera.
    /// </summary>
    /// <param name="point">The position of the point.</param>
    /// <param name="position">The position of the camera.</param>
    /// <param name="rotation">The direction of the camera, where 0 ± (n * 2 pi) is facing north. (Towards +X)</param>
    /// <param name="fov">The camera field of view in degrees.</param>
    /// <returns>Whether or not the point is visible from the camera, in the form of a confidence value between 0 (Certainly not visible) and 1. (Certainly visible)</returns>
    constexpr inline float is_point_visible(const glm::vec2& point, const glm::vec2& position, float rotation, float fov) {
        glm::vec2 relocated_point = rotate_point(point, position, -rotation) - position;
        if (relocated_point.x <= 0) return 0.0f;    // Can't see stuff behind the camera.

        float dmax  = relocated_point.x / std::tan(degrees_to_radians(90.0f - 0.5f * fov));
        float dreal = std::abs(relocated_point.y);
        if (dreal > dmax) return 0.0f;

        return std::clamp(10.0f * (1.0f - (dreal / dmax)), 0.0f, 1.0f);
    }


    
    constexpr inline glm::vec2 calculate_world_position(const glm::vec2& pos, float rot, float fov, float display_pos, float depth) {
        fov = degrees_to_radians(fov);

        float display_angle = (-0.5f * fov + (display_pos * fov));
        glm::vec2 origin_pos = glm::vec2{ std::cos(display_angle), std::sin(display_angle) } * depth;

        origin_pos = rotate_point(origin_pos, { 0, 0 }, rot) + pos;
        return origin_pos;
    }



    inline float distance_squared(const glm::vec3& a, const glm::vec3& b) {
        auto delta = b - a;
        return glm::dot(delta, delta);
    }
}