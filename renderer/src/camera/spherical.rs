use nalgebra_glm as na;

pub fn spherical_to_cartesian(r: f32, theta: f32, phi: f32) -> na::Vec3 {
    na::vec3(
        r * (phi.cos() * theta.cos()),
        -r * phi.sin(),
        r * (phi.cos() * phi.sin()),
    )
}

pub fn spherical_to_cartesian_dphi(r: f32, theta: f32, phi: f32) -> na::Vec3 {
    na::vec3(
        r * (phi.cos() * theta.cos()),
        -r * phi.sin(),
        r * (phi.cos() * theta.sin()),
    )
}
