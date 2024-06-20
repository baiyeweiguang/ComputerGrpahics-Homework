// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_CAMERA_HPP_
#define GL_HOMEWORK_CAMERA_HPP_

// clang-format off
// std
#include <cstdint>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>
// clang-format on
#include <fmt/core.h>

namespace gl_hwk {

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
 public:
  Camera(const glm::vec3 &position, float focal_length, uint32_t width,
         uint32_t height,
         const glm::vec3 &world_up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0,
         float pitch = 0)
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

  auto getProjectionMatrix() -> glm::mat4 {
    return glm::perspective(fov_y_, width_ / height_, 0.1f, 100.0f);
  }

  auto getViewMatrix() -> glm::mat4 {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  auto setZoom(float zoom) -> void {
    zoom = glm::radians(zoom);
    focal_length_ = fovToIntrinsic(zoom, height_);
    glm::vec2 fov = intrinsicToFov(focal_length_, width_, height_);
    fov_x_ = fov.x;
    fov_y_ = fov.y;
  }

  auto getZoom() -> float { return glm::degrees(fov_y_); }

  auto setYaw(float yaw) -> void {
    yaw_ = yaw;
    updateCameraVectors();
  }

  auto setPitch(float pitch) -> void {
    pitch_ = pitch;
    updateCameraVectors();
  }

  auto move(const glm::vec3 &vec) -> void { position_ += vec; }

  auto turnYaw(float angle) -> void {
    yaw_ += angle;
    updateCameraVectors();
  }

  auto turnPitch(float angle) -> void {
    pitch_ += angle;
    updateCameraVectors();
  }

  auto getUp() -> glm::vec3 { return up_; }

  auto getFront() -> glm::vec3 { return front_; }

  auto getRight() -> glm::vec3 { return right_; }

  auto getLeft() -> glm::vec3 { return -right_; }

  auto getPosition() -> glm::vec3 { return position_; }

 private:
  static auto intrinsicToFov(float focal, float w, float h) -> glm::vec2 const {
    float fov_x = 2 * atan(w / (2 * focal));
    float fov_y = 2 * atan(h / (2 * focal));
    return glm::vec2(fov_x, fov_y);
  }

  static auto fovToIntrinsic(float fov_y, float h) -> float {
    return h / tan(fov_y / 2) / 2;
  }

  auto updateCameraVectors() -> void {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

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
}  // namespace gl_hwk

#endif