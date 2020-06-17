#pragma once

#include <window/3d/primitive.hpp>
#include <window/main_window.hpp>
#include <vision/partition_space.hpp>
#include <utility/math.hpp>

#include <vector>
#include <cstddef>
#include <queue>
#include <unordered_set>


namespace robotica {
    struct weighted_graph {
        const vertex* start;

        struct edge {
            const vertex *a, *b;
            float cost;
        };

        std::vector<edge> edges;
    };


    inline weighted_graph create_graph(const partition_space<vertex>& verts, const vertex* start) {
        auto& settings = main_window::instance();

        std::queue<const vertex*> todo;
        std::unordered_set<const vertex*> done;

        std::vector<weighted_graph::edge> edges;


        todo.push(start);
        while (todo.size()) {
            const vertex* v = todo.front();
            todo.pop();

            auto nearby = verts.nearby(v->position, settings.max_path_length, { true, false, false });

            if (nearby.size() > settings.max_path_edges) {
                std::sort(nearby.begin(), nearby.end(), [&](const auto& a, const auto& b) { return distance_squared(a.get().position, v->position) < distance_squared(b.get().position, v->position); });
                nearby.erase(nearby.begin() + settings.max_path_edges, nearby.end());
            }


            const float limit_squared = settings.max_path_length * settings.max_path_length;
            for (const auto& vert : nearby) {
                const auto& pa = vert.get().position;
                const auto& pb = v->position;

                if (done.contains(&vert.get())) continue;
                if (distance_squared(pa, pb) > limit_squared) continue;
                if (std::abs((pb.y - pa.y) / (pb.x - pa.x)) > settings.max_path_slope) continue;

                edges.push_back({
                    v,
                    &vert.get(),
                    distance_squared(pa, pb)
                });

                todo.push(&vert.get());
            }

            done.insert(v);
        }

        return weighted_graph {
            start,
            std::move(edges)
        };
    }
}