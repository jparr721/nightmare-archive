#pragma once

#include "geometry.h"
#include "math.h"

namespace nm {
    class Camera {
    public:
        bool zooming = false;
        bool panning = false;
        bool rotating = false;

        real near_plane = 0.1;
        real far_plane = 1000.0;
        real rotation_sensitivity = 0.01;
        real pan_sensitivity = 0.01;
        real zoom_sensitivity = 0.1;
        real fovmin = 10.0;
        real fovmax = 120.0;
        real fov = 65.0;
        real radiusmin = 0.1;
        real radiusmax = 1000.0;
        real aspect_ratio = 4.0 / 3.0;

        Camera() = default;
        Camera(real near_plane = 0.1, real far_plane = 1000.0, real rotation_sensitivity = 0.01,
               real pan_sensitivity = 0.01, real zoom_sensitivity = 0.1, real fovmin = 10.0, real fovmax = 120.0,
               real fov = 65.0, real radiusmin = 0.1, real radiusmax = 1000.0, real aspect_ratio = 4.0 / 3.0)
            : near_plane(near_plane), far_plane(far_plane), rotation_sensitivity(rotation_sensitivity),
              pan_sensitivity(pan_sensitivity), zoom_sensitivity(zoom_sensitivity), fovmin(fovmin), fovmax(fovmax),
              fov(fov), radiusmin(radiusmin), radiusmax(radiusmax), aspect_ratio(aspect_ratio) {}

        void reset();
        void resize(real width, real height);
        void zoom(real dr);
        void pan(real du, real dv);
        void rotate(real du, real dv);
        void setPerspective();

        auto viewMatrix() -> mat4r &;
        auto projectionMatrix() -> mat4r &;

    private:
        real r_ = 1.0;
        real theta_ = M_PI / 2.0;
        real phi_ = M_PI / 2.0;

        vec3r displacement_ = vec3r::Zero();
        vec3r eye_ = vec3r::Zero();
        vec3r center_ = vec3r::Zero();
        vec3r up_ = vec3r(0, 1, 0);

        mat4r view_matrix_ = mat4r::Identity();
        mat4r projection_matrix_ = mat4r::Identity();

        void compile();

        auto leftDirection() -> vec3r;
        auto downDirection() -> vec3r;
    };
}// namespace nm
