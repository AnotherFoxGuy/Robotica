#pragma once

#include <vector>
#include <array>
#include <utility>


namespace robotica {
    struct maze_vertex {
        enum direction { NORTH, SOUTH, EAST, WEST };

        struct edge {
            int dest_index;
            int distance;
            direction ab;
        };
        std::vector<edge> edges;
    };


    struct mazemap {
        maze_vertex* begin;
        std::vector<maze_vertex> vertices;
    };


    inline maze_vertex::direction inverse(maze_vertex::direction d) {
        if (d == maze_vertex::NORTH) return maze_vertex::SOUTH;
        if (d == maze_vertex::SOUTH) return maze_vertex::NORTH;
        if (d == maze_vertex::EAST)  return maze_vertex::WEST;
        if (d == maze_vertex::WEST)  return maze_vertex::EAST;
    }


    inline mazemap generate_map(void) {
        // See mazemap.png
        const static std::array edges = {
            std::tuple { 0,  1,  maze_vertex::EAST,  3800 },
            std::tuple { 1,  2,  maze_vertex::SOUTH, 3300 },
            std::tuple { 1,  3,  maze_vertex::EAST,  2300 },
            std::tuple { 3,  4,  maze_vertex::EAST,  3300 },
            std::tuple { 3,  5,  maze_vertex::NORTH, 5100 },
            std::tuple { 5,  11, maze_vertex::NORTH, 2100 },
            std::tuple { 11, 12, maze_vertex::EAST,  3500 },
            std::tuple { 5,  6,  maze_vertex::WEST,  3600 },
            std::tuple { 6,  7,  maze_vertex::SOUTH, 2000 },
            std::tuple { 6,  8,  maze_vertex::WEST,  3500 },
            std::tuple { 8,  9,  maze_vertex::SOUTH, 1000 },
            std::tuple { 8,  10, maze_vertex::NORTH, 2100 }
        };

        mazemap result;
        for (int i = 0; i <= 12; ++i) result.vertices.push_back(maze_vertex{ });

        auto& verts = result.vertices;
        for (const auto& [a, b, dir, dist] : edges) {
            verts[a].edges.push_back({ b, dist, dir });
            verts[b].edges.push_back({ a, dist, inverse(dir) });
        }

        return result;
    }
}