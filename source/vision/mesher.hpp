#pragma once

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <algorithm>


namespace robotica {
    inline mesh create_mesh(const pointcloud& pc) {
        using kernel         = CGAL::Exact_predicates_inexact_constructions_kernel;
        using reconstruction = CGAL::Scale_space_surface_reconstruction_3<kernel>;
        using point          = typename kernel::Point_3;
        using facet          = reconstruction::Facet_iterator::value_type;


        std::vector<point> points;
        points.resize(pc.size());

        std::transform(
            std::execution::par_unseq,
            pc.begin(),
            pc.end(),
            points.begin(),
            [](const vertex& v) {
                return point { v.position.x, v.position.y, v.position.z };
            }
        );


        reconstruction rs(points.begin(), points.end());
        rs.increase_scale(1);
        rs.reconstruct_surface();


        mesh result;
        result.resize(rs.number_of_facets());

        std::transform(
            std::execution::par_unseq,
            rs.facets_begin(),
            rs.facets_end(),
            result.begin(),
            [&](const facet& facet) {
                assert(facet.size() == 3);

                std::array<vertex, 3> verts;
                for (int i = 0; i < 3; ++i) {
                    verts[i] = vertex {{
                        (rs.points_begin() + facet[i])->x(),
                        (rs.points_begin() + facet[i])->y(),
                        (rs.points_begin() + facet[i])->z()
                    }};
                }

                return triangle { verts[0], verts[1], verts[2] };
            }
        );

        return result;
    }
}