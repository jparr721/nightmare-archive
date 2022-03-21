#include "geometry.h"


namespace nm {
    vec3r sphericalToCartesian(real r, real theta, real phi) {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return vec3r(r * (cos_theta * sin_phi), r * cos_phi, r * (sin_theta * sin_phi));
    }

    vec3r sphericalToCartesianDPhi(real r, real theta, real phi) {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return vec3r(r * (cos_phi * cos_theta), -r * sin_phi, r * (cos_phi * sin_theta));
    }

    mat4r perspectiveProjection(real near, real far, real fov, real aspect) {
        const auto ymax = near * std::tan(fov * M_PI / 360.0);
        const auto xmax = ymax * aspect;

        const auto left = -xmax;
        const auto right = xmax;
        const auto bottom = -ymax;
        const auto top = ymax;

        const auto A = (right + left) / (right - left);
        const auto B = (top + bottom) / (top - bottom);
        const auto C = -(far + near) / (far - near);
        const auto D = -2.0 * far * near / (far - near);
        const auto E = 2.0 * near / (right - left);
        const auto F = 2.0 * near / (top - bottom);

        mat4r ret = mat4r::Zero();

        ret(0, 0) = E;

        ret(1, 1) = F;

        ret(2, 0) = A;
        ret(2, 1) = B;
        ret(2, 2) = C;
        ret(2, 3) = -1.0;

        ret(3, 2) = D;
        return ret;
    }

    mat4r lookAt(const vec3r &eye, const vec3r &at, const vec3r &up) {
        const vec3r forward = (at - eye).normalized();
        const vec3r side = forward.cross(up).normalized();
        const vec3r up_ = side.cross(forward).normalized();

        mat4r ret = mat4r::Zero();

        ret.row(0) << side(0), up_(0), -forward(0), 0.0;
        ret.row(1) << side(1), up_(1), -forward(1), 0.0;
        ret.row(2) << side(2), up_(2), -forward(2), 0.0;
        ret.row(3) << -side.dot(eye), -up.dot(eye), forward.dot(eye), 1.0;

        return ret;
    }
}// namespace nm
