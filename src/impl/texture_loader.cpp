#include "gl_homework/texture_loader.hpp"

#include <filesystem>
#include <opencv2/opencv.hpp>
#include <unordered_map>

namespace gl_hwk {

struct TextureInfo {
  GLuint id;
  std::vector<cv::Mat> textures;
  int type;
};

class TextureLoaderImpl {
 public:
  TextureLoaderImpl() = default;
  std::unordered_map<GLuint, TextureInfo> textures_;
};

TextureLoader::TextureLoader() : impl_(make_unique_impl<TextureLoaderImpl>()) {}

auto TextureLoader::instance() -> TextureLoader& {
  static TextureLoader instance;
  return instance;
}

auto TextureLoader::loadTexture(const std::string& texture_path) -> GLuint {
  if (!std::filesystem::exists(texture_path)) {
    fmt::print("TextureLoader: Texture file not found: {}\n", texture_path);
    return 0;
  }
  cv::Mat image = cv::imread(texture_path, cv::IMREAD_COLOR);

  if (image.empty()) {
    fmt::print("TextureLoader: Failed to load image: {}\n", texture_path);
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

  impl_->textures_[texture_id] = {
      texture_id, {std::move(image)}, GL_TEXTURE_2D};

  return texture_id;
}

auto TextureLoader::loadBoxMap(const std::vector<std::string>& paths)
    -> GLuint {
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  impl_->textures_[texture_id] = {texture_id, {}, GL_TEXTURE_CUBE_MAP};

  for (unsigned int i = 0; i < paths.size(); i++) {
    const std::string& path = paths[i];
    if (!std::filesystem::exists(path)) {
      fmt::print("TextureLoader: Texture file not found: {}\n", path);
      return 0;
    }

    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);

    if (image.empty()) {
      fmt::print("TextureLoader: Failed to load image: {}\n", path);
      return 0;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.cols,
                 image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.data);

    impl_->textures_[texture_id].textures.push_back(std::move(image));
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return texture_id;
}

auto TextureLoader::activeTexture(GLuint texture_id, int idx) -> void {
  if (impl_->textures_.find(texture_id) == impl_->textures_.end()) {
    fmt::print("TextureLoader: Texture ID not found: {}\n", texture_id);
    return;
  }
  int type = impl_->textures_[texture_id].type;
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(type, texture_id);
}

}  // namespace gl_hwk