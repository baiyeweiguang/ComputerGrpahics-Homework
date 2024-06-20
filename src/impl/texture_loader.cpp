#include "gl_homework/texture_loader.hpp"

#include <opencv2/opencv.hpp>
#include <unordered_map>

namespace gl_hwk {
class TextureLoaderImpl {
 public:
  TextureLoaderImpl() = default;
  std::unordered_map<GLuint, cv::Mat> textures_;
};

TextureLoader::TextureLoader() : impl_(make_unique_impl<TextureLoaderImpl>()) {}

auto TextureLoader::instance() -> TextureLoader& {
  static TextureLoader instance;
  return instance;
}

auto TextureLoader::loadTexture(const std::filesystem::path& texture_path)
    -> GLuint {
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

  impl_->textures_[texture_id] = image;

  return texture_id;
}

auto TextureLoader::activeTexture(GLuint texture_id, int GL_TEXTURE_idx)
    -> void {
  if (impl_->textures_.find(texture_id) == impl_->textures_.end()) {
    fmt::print("TextureLoader: Texture not found: {}\n", texture_id);
    return;
  }
  glActiveTexture(GL_TEXTURE0 + GL_TEXTURE_idx);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

}  // namespace gl_hwk