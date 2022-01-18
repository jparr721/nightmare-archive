use crate::camera::camera_base::CameraBase;
use crate::camera::spherical::{spherical_to_cartesian, spherical_to_cartesian_dphi};
use nalgebra_glm as na;

/// Super basic camera class, platform agnostic.
/// # Examples
/// ```no_run
/// use renderer::camera::basic_camera::BasicCamera;
///
/// // Make a basic camera
/// let basic_camera: BasicCamera = Default::default();
/// // Now, embed it anywhere with the operations
/// ```
#[derive(Debug, PartialEq)]
pub struct BasicCamera {
    pub zooming: bool,
    pub panning: bool,
    pub rotating: bool,

    view_matrix: na::Mat4,
    projection_matrix: na::Mat4,

    eye: na::Vec3,
    center: na::Vec3,
    up: na::Vec3,

    near_plane: f32,
    far_plane: f32,
    rotation_sensitivity: f32,
    pan_sensitivity: f32,
    zoom_sensitivity: f32,
    min_fov: f32,
    max_fov: f32,
    fov: f32,
    min_radius: f32,
    max_radius: f32,
    aspect_ratio: f32,

    displacement: na::Vec3,

    r: f32,
    theta: f32,
    phi: f32,
}

impl Default for BasicCamera {
    fn default() -> Self {
        BasicCamera {
            zooming: false,
            panning: false,
            rotating: false,

            view_matrix: na::identity(),
            projection_matrix: na::identity(),

            eye: na::zero(),
            center: na::zero(),
            up: na::zero(),

            near_plane: 0.1,
            far_plane: 1000.0,
            rotation_sensitivity: 0.01,
            pan_sensitivity: 0.01,
            zoom_sensitivity: 0.1,
            min_fov: 10.0,
            max_fov: 120.0,
            fov: 65.0,
            min_radius: 0.1,
            max_radius: 1000.0,
            aspect_ratio: (4.0 / 3.0),

            displacement: na::zero(),

            r: 1.0,
            theta: na::pi::<f32>() / 2.0,
            phi: na::pi::<f32>() / 2.0,
        }
    }
}

impl CameraBase for BasicCamera {
    /// Compute left as the normalized cross product of the current direction we're facing and
    /// the up direction in 3D euclidean space.
    fn left(&mut self) -> na::Vec3 {
        self.compile_transforms();
        let dir = na::normalize(&(self.center - self.eye));
        na::normalize(&na::cross(&dir, &self.up))
    }

    /// Computes the down direction of the current camera perspective. We first compile the
    /// transforms before moving on as `self.up` value may have changed.
    fn down(&mut self) -> na::Vec3 {
        self.compile_transforms();
        -self.up
    }

    fn view_matrix(&mut self) -> na::Mat4 {
        self.compile_transforms();
        self.view_matrix
    }

    fn projection_matrix(&mut self) -> na::Mat4 {
        self.compile_transforms();
        self.projection_matrix
    }

    fn zoom(&self, dz: f32) {}

    fn pan(&self, du: f32, dv: f32) {}

    fn rotate(&self, du: f32, dv: f32) {}

    fn compile_transforms(&mut self) {
        self.center = na::zero();
        self.eye = spherical_to_cartesian(self.r, self.theta, self.phi);
        self.up = na::normalize(&spherical_to_cartesian_dphi(self.r, self.theta, self.phi));

        // --------------------------------------------------------------------------------
        // Invert the up direction (since the spherical coordinates have phi
        // increasing downwards. Therefore we would like to have the (vector)
        // direction of the derivative inversed.
        // --------------------------------------------------------------------------------
        self.up = -self.up;
        self.center += self.displacement;
        self.eye += self.displacement;

        self.view_matrix = na::look_at(&self.eye, &self.center, &self.up);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_makes_default_basic_camera() {
        let basic_camera: BasicCamera = Default::default();

        let compare = BasicCamera {
            zooming: false,
            panning: false,
            rotating: false,

            view_matrix: na::identity(),
            projection_matrix: na::identity(),

            eye: na::zero(),
            center: na::zero(),
            up: na::zero(),

            near_plane: 0.1,
            far_plane: 1000.0,
            rotation_sensitivity: 0.01,
            pan_sensitivity: 0.01,
            zoom_sensitivity: 0.1,
            min_fov: 10.0,
            max_fov: 120.0,
            fov: 65.0,
            min_radius: 0.1,
            max_radius: 1000.0,
            aspect_ratio: (4.0 / 3.0),

            displacement: na::zero(),

            r: 1.0,
            theta: na::pi::<f32>() / 2.0,
            phi: na::pi::<f32>() / 2.0,
        };
        assert_eq!(compare, basic_camera);
    }
}
