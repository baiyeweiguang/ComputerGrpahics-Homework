#include "gl_homework/camera.hpp"

// clang-format off
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fmt/core.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
// clang-format on

namespace gl_hwk {

class CameraImpl {
 public:
  CameraImpl() = default;
  CameraImpl(const glm::vec3 &position, float focal_length, uint32_t width, uint32_t height, const glm::vec3 &world_up,
             float yaw, float pitch);
  auto intrinsicToFov(float focal, float w, float h) const -> glm::vec2;
  auto fovToIntrinsic(float fov_y, float h) const -> float;
  auto updateCameraVectors() -> void;

  float focal_length_;
  float width_;
  float height_;

  float fov_x_;
  float fov_y_;

  float yaw_;
  float pitch_;
  glm::vec3 position_;
  glm::vec3 world_up_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
};

CameraImpl::CameraImpl(const glm::vec3 &position, float focal_length, uint32_t width, uint32_t height,
                       const glm::vec3 &world_up, float yaw, float pitch)
    : position_(position),
      focal_length_(focal_length),
      width_(width),
      height_(height),
      world_up_(world_up),
      yaw_(yaw),
      pitch_(pitch) {
  updateCameraVectors();
  glm::vec2 fov = intrinsicToFov(focal_length, width, height);
  fov_x_ = fov.x;
  fov_y_ = fov.y;
}

auto CameraImpl::intrinsicToFov(float focal, float w, float h) const -> glm::vec2 {
  float fov_x = 2 * atan(w / (2 * focal));
  float fov_y = 2 * atan(h / (2 * focal));
  return glm::vec2(fov_x, fov_y);
}

auto CameraImpl::fovToIntrinsic(float fov_y, float h) const -> float { return h / (2 * tan(fov_y / 2)); }

auto CameraImpl::updateCameraVectors() -> void {
  glm::vec3 front;
  front.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front);

  right_ = glm::normalize(glm::cross(front_, world_up_));
  up_ = glm::normalize(glm::cross(right_, front_));
}

Camera::Camera(const glm::vec3 &position, float focal_length, uint32_t width, uint32_t height,
               const glm::vec3 &world_up, float yaw, float pitch) {
  impl_ = make_unique_impl<CameraImpl>(position, focal_length, width, height, world_up, yaw, pitch);
}

auto Camera::getProjectionMatrix() -> glm::mat4 {
  return glm::perspective(impl_->fov_y_, impl_->width_ / impl_->height_, 0.1f, 100.0f);
}

auto Camera::getViewMatrix() -> glm::mat4 {
  return glm::lookAt(impl_->position_, impl_->position_ + impl_->front_, impl_->up_);
}

auto Camera::setZoom(float zoom) -> void {
  zoom = glm::radians(zoom);
  impl_->focal_length_ = impl_->fovToIntrinsic(zoom, impl_->height_);
  glm::vec2 fov = impl_->intrinsicToFov(impl_->focal_length_, impl_->width_, impl_->height_);
  impl_->fov_x_ = fov.x;
  impl_->fov_y_ = fov.y;
}

auto Camera::getZoom() -> float { return glm::degrees(impl_->fov_y_); }

auto Camera::setYaw(float yaw) -> void {
  impl_->yaw_ = yaw;
  impl_->updateCameraVectors();
}

auto Camera::setPitch(float pitch) -> void {
  impl_->pitch_ = pitch;
  impl_->updateCameraVectors();
}

auto Camera::move(const glm::vec3 &vec) -> void { impl_->position_ += vec; }

auto Camera::turnYaw(float angle) -> void {
  impl_->yaw_ += angle;
  impl_->updateCameraVectors();
}

auto Camera::turnPitch(float angle) -> void {
  impl_->pitch_ += angle;
  impl_->updateCameraVectors();
}

auto Camera::getUp() -> glm::vec3 { return impl_->up_; }

auto Camera::getFront() -> glm::vec3 { return impl_->front_; }

auto Camera::getRight() -> glm::vec3 { return impl_->right_; }

auto Camera::getLeft() -> glm::vec3 { return -(impl_->right_); }

auto Camera::getPosition() -> glm::vec3 { return impl_->position_; }

}  // namespace gl_hwk