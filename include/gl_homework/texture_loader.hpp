// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_CAMERA_HPP_
#define GL_HOMEWORK_CAMERA_HPP_

// clang-format off
// std
#include <unordered_map>
#include <filesystem>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
// third party
#include <fmt/core.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
// clang-format on

namespace gl_hwk {
class TextureLoader {
 public:
  TextureLoader() = default;

  auto loadTexture(const std::filesystem::path& texture_path) -> GLuint {
    if (!std::filesystem::exists(texture_path)) {
      fmt::print("TextureLoader: Texture file not found: {}\n",
                 texture_path.string());
      return 0;
    }
    cv::Mat image = cv::imread(texture_path.string(), cv::IMREAD_COLOR);

    if (image.empty()) {
      fmt::print("TextureLoader: Failed to load image: {}\n",
                 texture_path.string());
      return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR_EXT,
                 GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    textures_[texture_id] = image;

    return texture_id;
  }

  static auto activeTexture(GLuint texture_id, int GL_TEXTURE_idx) -> void {
    glActiveTexture(GL_TEXTURE_idx);
    glBindTexture(GL_TEXTURE_2D, texture_id);
  }

 private:
  std::unordered_map<GLuint, cv::Mat> textures_;
};

}  // namespace gl_hwk
#endif
