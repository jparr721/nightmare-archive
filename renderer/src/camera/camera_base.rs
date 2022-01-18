use nalgebra_glm as na;

pub trait CameraBase {
    fn left(&mut self) -> na::Vec3;
    fn down(&mut self) -> na::Vec3;

    fn view_matrix(&mut self) -> na::Mat4;
    fn projection_matrix(&mut self) -> na::Mat4;

    fn zoom(&self, dz: f32);
    fn pan(&self, du: f32, dv: f32);
    fn rotate(&self, du: f32, dv: f32);
    fn compile_transforms(&mut self);
}
