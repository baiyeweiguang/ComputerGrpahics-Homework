// Copyright 2024 Chengfu Zou

#ifndef GL_HOMEWORK_TEXTURE_LOADER_HPP_
#define GL_HOMEWORK_TEXTURE_LOADER_HPP_

// clang-format off
// std
#include <vector>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
// third party
#include <fmt/core.h>
// project
#include "gl_homework/impl.hpp"
// clang-format on

namespace gl_hwk {
class TextureLoaderImpl;
// Singleton
class TextureLoader {
 public:
  static auto instance() -> TextureLoader&;

  auto loadTexture(const std::string& texture_path) -> GLuint;

  auto loadBoxMap(const std::vector<std::string>& texture_paths) -> GLuint;

  auto activeTexture(GLuint texture_id, int idx) -> void;

 private:
  TextureLoader();
  // disable copy and move
  TextureLoader(const TextureLoader&) = delete;
  TextureLoader& operator=(const TextureLoader&) = delete;
  TextureLoader(TextureLoader&&) = delete;
  TextureLoader& operator=(TextureLoader&&) = delete;

  UniqueImpl<TextureLoaderImpl> impl_;
};

}  // namespace gl_hwk
#endif
