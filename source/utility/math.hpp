#pragma once

#include <utility/typedefs.hpp>

#include <cmath>


namespace robotica {
    constexpr inline long double pi = 3.14159265358979323846;

    constexpr inline u64 au_length = 149'597'870'700;
    constexpr inline u64 parsec_length = (180 * 60 * 60 * au_length) / pi;


    template <typename T> constexpr inline T degrees_to_radians(const T& v) {
        return 2.0 * pi * (v / 360.0);
    }


    // Calculate the distance of an object using the parallax.
    // center_dist: The distance from the center of the screen to the edge of the fov. In range [0, 1]
    // camera_dist: The distance between the cameras.
    // fov: The fov of the cameras.
    constexpr inline float calculate_parallax_distance(float center_dist_a, float center_dist_b, float camera_dist, float fov) {
        auto to_arcseconds = [](const auto& radians) constexpr { return radians / (pi / 648000);  };
        auto to_meters = [](const auto& parsecs) constexpr { return parsecs * parsec_length; };

        const float angle_a = degrees_to_radians(center_dist_a / fov);
        const float angle_b = degrees_to_radians(center_dist_b / fov);

        const long double p = to_arcseconds(std::atan(0.5 * (angle_a + angle_b)));
        const long double d = to_meters(1.0 / p);

        return d;
    }
}