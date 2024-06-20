// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_CAMERA_HPP_
#define GL_HOMEWORK_CAMERA_HPP_

// clang-format off
// std
#include <cstdint>
// OpenGL
#include <glm/glm.hpp>
// project
#include "gl_homework/impl.hpp"
// clang-format on

namespace gl_hwk {

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class CameraImpl;
class Camera {
 public:
  Camera(const glm::vec3 &position, float focal_length, uint32_t width,
         uint32_t height,
         const glm::vec3 &world_up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0,
         float pitch = 0);

  auto getProjectionMatrix() -> glm::mat4;
  auto getViewMatrix() -> glm::mat4;
  auto setZoom(float zoom) -> void;
  auto getZoom() -> float;
  auto setYaw(float yaw) -> void;
  auto setPitch(float pitch) -> void;
  auto move(const glm::vec3 &vec) -> void;
  auto turnYaw(float angle) -> void;
  auto turnPitch(float angle) -> void;

  auto getUp() -> glm::vec3;
  auto getFront() -> glm::vec3;
  auto getRight() -> glm::vec3;
  auto getLeft() -> glm::vec3;
  auto getPosition() -> glm::vec3;

 private:
  // 隐藏实现
  UniqueImpl<CameraImpl> impl_;
};

}  // namespace gl_hwk

#endif  // GL_HOMEWORK_CAMERA_HPP_
