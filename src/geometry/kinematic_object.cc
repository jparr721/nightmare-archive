#include "kinematic_object.h"

namespace nm::geometry {
    auto localVertexToWorld(const vec3 &local, const KinematicObject &kinematicObject) -> vec3 {
        return kinematicObject.rotation * kinematicObject.scale * local + kinematicObject.translation;
    }

    auto localNormalToWorld(const vec3 &normal, const KinematicObject &kinematicObject) -> vec3 {
        return kinematicObject.rotation * normal;
    }

    auto worldVertexToLocal(const vec3 &world, const KinematicObject &kinematicObject) -> vec3 {
        return kinematicObject.scaleInverse * kinematicObject.rotation.transpose() *
               (world - kinematicObject.translation);
    }
}// namespace nm::geometry