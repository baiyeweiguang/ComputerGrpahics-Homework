#include "gl_homework/skybox.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <memory>

#include "gl_homework/camera.hpp"
#include "gl_homework/impl.hpp"
#include "gl_homework/primitive_builder.hpp"
#include "gl_homework/texture_loader.hpp"

namespace gl_hwk {

class SkyBoxImpl {
 public:
  SkyBoxImpl(GLuint texture_id, std::shared_ptr<Shader> shader,
             std::shared_ptr<Camera> camera);

  auto draw() -> void;
  std::shared_ptr<Shader> shader_;
  std::shared_ptr<Camera> camera_;
  std::shared_ptr<PrimitiveBuilder> builder_;
  std::vector<glm::vec3> vertices_;
  GLuint texture_id_;
};

SkyBoxImpl::SkyBoxImpl(GLuint texture_id, std::shared_ptr<Shader> shader,
                       std::shared_ptr<Camera> camera)
    : shader_(shader), camera_(camera), texture_id_(texture_id) {
  vertices_ = std::vector<glm::vec3>{
      {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
      {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},

      {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
      {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

      {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
      {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},

      {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
      {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

      {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, 1.0f},
      {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},

      {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, -1.0f},
      {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f}};

  builder_ = std::make_shared<PrimitiveBuilder>(shader);
  shader->start();
  shader->setInt("skybox", 0);
}

auto SkyBoxImpl::draw() -> void {
  // glClear(GL_DEPTH_BUFFER_BIT);
  glDepthFunc(GL_LEQUAL);
  shader_->start();
  gl_hwk::TextureLoader::instance().activeTexture(texture_id_, 0);
  shader_->setMat4("projection", camera_->getProjectionMatrix());
  shader_->setMat4("view", glm::mat4(glm::mat3(camera_->getViewMatrix())));
  builder_->buildTriangles("skybox", vertices_, {}, {});
  glClear(GL_DEPTH_BUFFER_BIT);
  glDepthFunc(GL_LESS);
}

SkyBox::SkyBox(const std::vector<std::string>& paths,
               std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) {
  GLuint texture_id = gl_hwk::TextureLoader::instance().loadBoxMap(paths);
  impl_ = make_unique_impl<SkyBoxImpl>(texture_id, shader, camera);
}

auto SkyBox::draw() -> void { impl_->draw(); }

auto SkyBox::setShader(std::shared_ptr<Shader> shader) -> void {
  impl_->shader_ = shader;
  impl_->builder_->setShader(shader);
}

}  // namespace gl_hwk