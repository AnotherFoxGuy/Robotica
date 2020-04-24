#pragma once

#include <utility/typedefs.hpp>

#include <glm/glm.hpp>

#include <cmath>


namespace robotica {
    constexpr inline long double pi = 3.14159265358979323846;

    constexpr inline u64 au_length = 149'597'870'700;
    constexpr inline u64 parsec_length = (180 * 60 * 60 * au_length) / pi;


    template <typename T> constexpr inline T degrees_to_radians(const T& v) {
        return 2.0 * pi * (v / 360.0);
    }


    // Calculate the position of an object using its parallax.
    // Assumes the baseline is centered at camera_pos and a camera_rot of 0 means the camera faces to +Y.
    // dA and dB are the positions of the point on cameras A and B respectively, in the range [-1, 1],
    // where -1 is all the way on the left of the screen, and 1 is all the way right on the screen.
    // b is the baseline distance, aka. the distance between the cameras.
    // Please note that while FOV is in degrees, camera_rot must be provided in radians.
    constexpr inline glm::vec2 calculate_parallax_position(float dA, float dB, float b, float fov, glm::vec2 camera_pos, float camera_rot) {
        const float fov_plane_angle = degrees_to_radians(270 - (0.5 * fov));

        // The angle of the object, X, to the baseline as seen from each camera.
        // Given a triangle with as sides the view vector of the camera, the fov line and a line between them with length 1,
        // the length of the camera view vector, L, is tan(x) with x the angle between the fov line and the length 1 line.
        // Given the endpoints of L, Lc and Le, the angle can be computed as the angle between the edges (Lc, Le) and (Lc, X)
        // in the triangle (Lc, Le, X).
        const float theta_a = (0.5 * pi) - std::atan(dA / std::tan(fov_plane_angle));
        const float theta_b = (0.5 * pi) - std::atan(dB / std::tan(fov_plane_angle));

        // The angle between the two view lines from each camera to the point.
        const float theta_c = pi - theta_a - theta_b;

        // Law of sines: A / sin(theta_a) = B / sin(theta_b) = C / sin(theta_c) where C is the baseline.
        // This gives us the length of each view line, La and Lb.
        const float sine_constant = b / std::sin(theta_c);
        const float La = sine_constant * std::sin(theta_a);
        const float Lb = sine_constant * std::sin(theta_b);

        // Each view line can be used to calculate the position, so just average both to minimize errors.
        const float distances[2]  = { La, Lb };
        const float camoffsets[2] = { -0.5f * b, 0.5f * b };
        const float angles[2]     = { theta_a, theta_b };
        glm::vec2 results[2];

        for (int i = 0; i < 2; ++i) {
            const float a = angles[i] + camera_rot;

            glm::vec2 heading = glm::normalize(glm::vec2{ std::cos(a), std::sin(a) });
            heading *= distances[i];

            results[i] = heading + camera_pos + camoffsets[i];
        }

        return 0.5f * (results[0] + results[1]);
    }
}