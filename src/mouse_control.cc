#include "mouse_control.h"
#include <igl/ray_mesh_intersect.h>
#include <igl/unproject.h>

namespace nm {
    auto shootRay(const matXr &vertices, const matXi &faces, const vec3r &start, const vec3r &dir)
            -> std::optional<igl::Hit> {
        std::vector<igl::Hit> hits;
        if (!igl::ray_mesh_intersect(start, dir, vertices, faces, hits)) { return std::nullopt; }

        // Get the first hit.
        return hits.at(0);
    }

    auto pickNearestVertex(const vec3r &win, const mat4r &view, const mat4r &proj, const vec4r &viewport,
                           const matXr &vertices, const matXi &faces, double radius) -> int {
        // Source, destination and direction in world
        vec3r start, dir;
        vec3r win0(win(0), win(1), win(2));
        vec3r win1(win(0), win(1), 1.);


        igl::unproject(win0, view, proj, viewport, start);
        igl::unproject(win1, view, proj, viewport, dir);
        dir -= start;

        const auto hit = shootRay(vertices, faces, start, dir);

        if (!hit.has_value()) { return -1; }

        vec3r bc(1.0 - hit.value().u - hit.value().v, hit.value().u, hit.value().v);
        unsigned int fid = hit.value().id;

        long c;
        bc.maxCoeff(&c);
        unsigned int vid = faces(fid, c);

        for (auto ii = 0; ii < vertices.rows(); ++ii) {
            if ((vertices.row(ii) - vertices.row(vid)).norm() < radius) { return ii; }
        }

        return -1;
    }

}// namespace nm