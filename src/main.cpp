// Copyright 2024 Chengfu Zou.

// clang-format off
// std
#include <functional>
#include <memory>
// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// third party
#include <fmt/core.h>
// project
#include "gl_homework/opengl_application.hpp"
#include "gl_homework/shader.hpp"
#include "gl_homework/primitive_builder.hpp"
#include "gl_homework/texture_loader.hpp"
// clang-format on

int main(int argc, char** argv) {
  gl_hwk::OpenGLApplication::instance().init(argc, argv);

  // auto shader = std::make_shared<gl_hwk::Shader>(
  //     "/home/zcf/Study/computer_graphics/CG_experiments/shader/"
  //     "pure_color.vert.GLSL",
  //     "/home/zcf/Study/computer_graphics/CG_experiments/shader/"
  //     "pure_color.frag.GLSL");

  auto shader = std::make_shared<gl_hwk::Shader>(
      "shader/"
      "texture.vert.GLSL",
      "shader/"
      "texture.frag.GLSL");

  gl_hwk::TextureLoader texture_loader;

  GLuint texture = texture_loader.loadTexture("texture/wall.jpg");
  shader->start();
  shader->setInt("texture1", 0);

  auto builder = gl_hwk::PrimitiveBuilder(shader);

  // clang-format off
  auto vertices = std::vector<glm::vec3>{
        {-0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f, -0.5f}, 
        { 0.5f,  0.5f, -0.5f}, 
        { 0.5f,  0.5f, -0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 

        {-0.5f, -0.5f,  0.5f}, 
        { 0.5f, -0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 

        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 

        {0.5f,  0.5f,  0.5f},
        {0.5f,  0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f,  0.5f},
        {0.5f,  0.5f,  0.5f},

        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f}
  };

  auto color = std::vector<glm::vec3>{     
    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f}};
  // clang-format on

  glm::mat4 view = glm::mat4(
      1.0f);  // make sure to initialize matrix to identity matrix first
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)520 / (float)520,
                                0.1f, 100.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  // pass transformation matrices to the shader
  shader->setMat4(
      "projection",
      projection);  // note: currently we set the projection matrix each frame,
                    // but since the projection matrix rarely changes it's often
                    // best practice to set it outside the main loop only once.
  shader->setMat4("view", view);

  auto cube_positions = std::vector<glm::vec3>{
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  auto render_func = [&]() -> void {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture_loader.activeTexture(texture, GL_TEXTURE0);

    for (int i = 0; i < 10; i++) {
      // calculate the model matrix for each object and pass it to shader
      // before
      // drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      builder.buildTriangleStrip(fmt::format("cube_{}", 0), vertices, {}, color,
                                 model);
    }
    glFlush();
  };

  gl_hwk::OpenGLApplication::instance().setRenderFunc(std::move(render_func));

  auto keyboardCallback = [](unsigned char key, int x, int y) {
    if (key == 27) {
      exit(0);
    }
  };

  gl_hwk::OpenGLApplication::instance().setKeyboardCallback(
      std::move(keyboardCallback));

  gl_hwk::OpenGLApplication::instance().run();

  return 0;
}
