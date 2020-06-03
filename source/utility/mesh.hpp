#pragma once

#include <glm/glm.hpp>

#include <vector>


namespace robotica {
    using vertex = glm::vec3;


    struct triangle {
        vertex a, b, c;
    };


    struct mesh {
        std::vector<triangle> triangles;
    };
}