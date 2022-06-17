#include "cube.h"

namespace nm::geometry::primitives {
    void cube_loadGeometry(mat &V, mati &F) {
        F.resize(12, 3);
        F.row(0) = vec3i(4, 7, 6);
        F.row(1) = vec3i(4, 6, 5);
        F.row(2) = vec3i(3, 0, 1);
        F.row(3) = vec3i(3, 1, 2);
        F.row(4) = vec3i(7, 3, 2);
        F.row(5) = vec3i(7, 2, 6);
        F.row(6) = vec3i(0, 4, 5);
        F.row(7) = vec3i(0, 5, 1);
        F.row(8) = vec3i(2, 1, 5);
        F.row(9) = vec3i(2, 5, 6);
        F.row(10) = vec3i(0, 3, 7);
        F.row(11) = vec3i(0, 7, 4);

        V.resize(8, 3);
        V.row(0) = vec3(0.0, 0.0, 0.0);
        V.row(1) = vec3(0.0, 1.0, 0.0);
        V.row(2) = vec3(1.0, 1.0, 0.0);
        V.row(3) = vec3(1.0, 0.0, 0.0);
        V.row(4) = vec3(0.0, 0.0, 1.0);
        V.row(5) = vec3(0.0, 1.0, 1.0);
        V.row(6) = vec3(1.0, 1.0, 1.0);
        V.row(7) = vec3(1.0, 0.0, 1.0);
    }

    auto cube_inside(const vec3 &point, const KinematicObject &cube) -> bool {
        // Transform back to local coordinates
        const vec3 transformed = worldVertexToLocal(point, cube);
        return transformed(0) <= 0.5 && transformed(0) >= -0.5 && transformed(1) <= 0.5 && transformed(1) >= -0.5 &&
               transformed(2) <= 0.5 && transformed(2) >= -0.5;
    }

    auto cube_distance(const vec3 &point, const KinematicObject &cube) -> real {
        // Transform back to local coordinates
        const vec3 transformed = worldVertexToLocal(point, cube);

        if (cube_inside(point, cube)) {
            const real xMin = std::min((0.5 - transformed(0)), (transformed(0) - (-0.5)));
            const real yMin = std::min((0.5 - transformed(1)), (transformed(1) - (-0.5)));
            const real zMin = std::min((0.5 - transformed(2)), (transformed(2) - (-0.5)));

            return std::abs(std::min(std::min(xMin, yMin), zMin));
        }

        vec3 diff = vec3::Zero();
        if (transformed(2) > 0.5) {
            diff(2) = transformed(2) - 0.5;
        } else if (transformed(2) < -0.5) {
            diff(2) = -0.5 - transformed(2);
        }

        if (transformed(1) > 0.5) {
            diff(1) = transformed(1) - 0.5;
        } else if (transformed(1) < -0.5) {
            diff(1) = -0.5 - transformed(1);
        }

        if (transformed(0) > 0.5) {
            diff(0) = transformed(0) - 0.5;
        } else if (transformed(0) < -0.5) {
            diff(0) = -0.5 - transformed(0);
        }

        return diff.norm() * cube.scale(0, 0);
    }

    auto cube_signedDistance(const vec3 &point, const KinematicObject &cube) -> real {
        // transform back to local coordinates
        const vec3 transformed = worldVertexToLocal(point, cube);

        if (cube_inside(point, cube)) {
            const real xMin = std::min((0.5 - transformed(0)), (transformed(0) - (-0.5)));
            const real yMin = std::min((0.5 - transformed(1)), (transformed(1) - (-0.5)));
            const real zMin = std::min((0.5 - transformed(2)), (transformed(2) - (-0.5)));

            return -fabs(std::min(std::min(xMin, yMin), zMin)) * cube.scale(0, 0);
        }

        // handle edges and points
        vec3 diff = vec3::Zero();
        if (transformed(2) > 0.5) {
            diff(2) = transformed(2) - 0.5;
        } else if (transformed(2) < -0.5) {
            diff(2) = -0.5 - transformed(2);
        }

        if (transformed(1) > 0.5) {
            diff(1) = transformed(1) - 0.5;
        } else if (transformed(1) < -0.5) {
            diff(1) = -0.5 - transformed(1);
        }

        if (transformed(0) > 0.5) {
            diff(0) = transformed(0) - 0.5;
        } else if (transformed(0) < -0.5) {
            diff(0) = -0.5 - transformed(0);
        }

        return diff.norm() * cube.scale(0, 0);
    }

    void cube_getClosestPoint(const vec3 &queryPoint, const KinematicObject &cube, vec3 &closestPointLocal,
                              vec3 &normalLocal) {
        const vec3 collisionPoint = worldVertexToLocal(queryPoint, cube);

        vec diffs(6);
        diffs(0) = 0.5 + collisionPoint[0];
        diffs(1) = 0.5 - collisionPoint[0];

        diffs(2) = 0.5 + collisionPoint[1];
        diffs(3) = 0.5 - collisionPoint[1];

        diffs(4) = 0.5 + collisionPoint[2];
        diffs(5) = 0.5 - collisionPoint[2];

        int minIndex = 0;
        real minFound = diffs(0);

        for (int x = 1; x < 6; x++) {
            if (diffs(x) < minFound) {
                minFound = diffs(x);
                minIndex = x;
            }
        }
        closestPointLocal = collisionPoint;
        closestPointLocal[0] = -0.5;
        normalLocal = vec3(-1, 0, 0);

        switch (minIndex) {
            case 1:
                closestPointLocal = collisionPoint;
                closestPointLocal[0] = 0.5;
                normalLocal = vec3(1, 0, 0);
                break;
            case 2:
                closestPointLocal = collisionPoint;
                closestPointLocal[1] = -0.5;
                normalLocal = vec3(0, -1, 0);
                break;
            case 3:
                closestPointLocal = collisionPoint;
                closestPointLocal[1] = 0.5;
                normalLocal = vec3(0, 1, 0);
                break;
            case 4:
                closestPointLocal = collisionPoint;
                closestPointLocal[2] = -0.5;
                normalLocal = vec3(0, 0, -1);
                break;
            case 5:
                closestPointLocal = collisionPoint;
                closestPointLocal[2] = 0.5;
                normalLocal = vec3(0, 0, 1);
                break;
        }
    }

}// namespace nm::geometry::primitives