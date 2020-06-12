#pragma once

#include <glm/glm.hpp>

#include <vector>


namespace robotica {
    struct vertex {
        glm::vec3 position;
    };


    struct triangle {
        vertex a, b, c;
    };


    using mesh       = std::vector<triangle>;
    using pointcloud = std::vector<vertex>;
}