#include "camera.h"

namespace nm {
    void Camera::reset() {
        displacement_ = vec3r::Zero();
        eye_ = vec3r::Zero();
        center_ = vec3r::Zero();
        up_ = vec3r(0, 1, 0);

        r_ = 1.0;
        real theta_ = M_PI / 2.0;
        real phi_ = M_PI / 2.0;
    }

    void Camera::resize(int width, int height) {
        aspect_ratio = width / height;
        setPerspective();
        compile();
    }

    void Camera::zoom(real dr) {
        if (r_ + dr > radiusmax) {
            r_ = radiusmax;
            compile();
            return;
        }
        if (r_ + dr < radiusmin) {
            r_ = radiusmin;
            compile();
            return;
        }

        r_ += dr;
        compile();
    }

    void Camera::pan(real du, real dv) {
        const vec3r u_dir = leftDirection();
        const vec3r v_dir = downDirection();

        const vec3r u_disp = (du * pan_sensitivity) * u_dir;
        const vec3r v_disp = (du * pan_sensitivity) * v_dir;
        const vec3r pan_disp = u_disp + v_disp;

        displacement_ += pan_disp;
        compile();
    }

    void Camera::rotate(real du, real dv) {
        theta_ -= du * rotation_sensitivity;
        phi_ += dv * rotation_sensitivity;
        compile();
    }

    void Camera::setPerspective() {
        if (fov > fovmax) { fov = fovmax; }
        if (fov < fovmin) { fov = fovmin; }

        projection_matrix_ = perspectiveProjection(near_plane, far_plane, fov, aspect_ratio);
    }

    auto Camera::viewMatrix() -> mat4r & {
        compile();
        return view_matrix_;
    }

    auto Camera::projectionMatrix() -> mat4r & {
        compile();
        return projection_matrix_;
    }

    /// ==========================================
    void Camera::compile() {
        center_ = vec3r::Zero();
        eye_ = sphericalToCartesian(r_, theta_, phi_);
        up_ = sphericalToCartesianDPhi(r_, theta_, phi_).normalized();

        // --------------------------------------------------------------------------------
        // Invert the up direction (since the spherical coordinates have phi
        // increasing downwards. Therefore we would like to have the (vector)
        // direction of the derivative inversed.
        // --------------------------------------------------------------------------------
        up_ *= -1.0;
        center_ += displacement_;
        eye_ += displacement_;
        view_matrix_ = lookAt(eye_, center_, up_);
    }

    auto Camera::leftDirection() -> vec3r {
        compile();
        const vec3r dir = (center_ - eye_).normalized();
        return dir.cross(up_).normalized();
    }

    auto Camera::downDirection() -> vec3r {
        compile();
        return -up_;
    }
}// namespace nm
