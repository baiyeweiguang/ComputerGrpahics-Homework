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
// 单例模式 
class TextureLoader {
 public:
  static auto instance() -> TextureLoader&;

  auto loadTexture(const std::string& texture_path, bool flip=false) -> GLuint;

  auto loadBoxMap(const std::vector<std::string>& texture_paths) -> GLuint;

  auto setTextureAlpha(GLuint texture_id, float alpha) -> void;

  auto activeTexture(GLuint texture_id, int idx) -> void;

 private:
  TextureLoader();
  // 禁止拷贝和移动 
  TextureLoader(const TextureLoader&) = delete;
  TextureLoader& operator=(const TextureLoader&) = delete;
  TextureLoader(TextureLoader&&) = delete;
  TextureLoader& operator=(TextureLoader&&) = delete;

  unique_impl<TextureLoaderImpl> impl_;
};

}  // namespace gl_hwk
#endif
