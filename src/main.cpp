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
#include "gl_homework/camera.hpp"
#include "gl_homework/primitive_builder.hpp"
#include "gl_homework/texture_loader.hpp"
// clang-format on

int main(int argc, char** argv) {
  // 初始化OpenGL
  gl_hwk::WindowOptions options;
  options.name = argv[0];
  options.width = 1024;
  options.height = 1024;

  gl_hwk::OpenGLApplication::instance().init(argc, argv, options);

  // 着色器
  auto shader = std::make_shared<gl_hwk::Shader>(
      "shader/"
      "texture.vert.GLSL",
      "shader/"
      "texture.frag.GLSL");

  // 纹理
  GLuint texture =
      gl_hwk::TextureLoader::instance().loadTexture("texture/wall.jpg");
  shader->start();
  shader->setInt("texture1", 0);

  // 顶点数据
  // clang-format off
  auto vertices = std::vector<glm::vec3>{
        // 背面
        {-0.5f, -0.5f, -0.5f}, // 左下
        { 0.5f, -0.5f, -0.5f}, // 右下
        { 0.5f,  0.5f, -0.5f}, // 右上
        { 0.5f,  0.5f, -0.5f}, // 右上
        {-0.5f,  0.5f, -0.5f}, // 左上
        {-0.5f, -0.5f, -0.5f}, // 左下
        // 正面
        {-0.5f, -0.5f,  0.5f}, 
        { 0.5f, -0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        { 0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, 0.5f}, 
        {-0.5f, -0.5f, 0.5f},  
        // 左面
        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 
        // 右面
        {0.5f,  0.5f,  0.5f},
        {0.5f,  0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f,  0.5f},
        {0.5f,  0.5f,  0.5f},
        // 底面
        {-0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f},
        // 顶面
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f},
  };

  auto color = std::vector<glm::vec3>{     
    { 0.0f, 0.0f, 0.0f}, // 左下
    { 1.0f, 0.0f, 0.0f}, // 右下
    { 1.0f, 1.0f, 0.0f}, // 右上
    { 1.0f, 1.0f, 0.0f}, // 右上
    { 0.0f, 1.0f, 0.0f}, // 左上
    { 0.0f, 0.0f, 0.0f}, // 左下

    { 0.0f, 0.0f, 0.0f},
    { 1.0f, 0.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 1.0f, 1.0f, 0.0f},
    { 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f, 0.0f},
    
    { 1.0f, 0.0f, 0.0f}, // 右下
    { 1.0f, 1.0f, 0.0f}, // 右上
    { 0.0f, 1.0f, 0.0f}, // 左上
    { 0.0f, 1.0f, 0.0f}, // 左上
    { 0.0f, 0.0f, 0.0f}, // 左下
    { 1.0f, 0.0f, 0.0f}, // 右下

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


  auto cube_positions = std::vector<glm::vec3>{
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  // clang-format on

  // Camera
  auto camera = std::make_shared<gl_hwk::Camera>(glm::vec3(0.0f, 0.0f, -3.0f),
                                                 600.f, 1024, 1024);

  // Builder
  auto builder = std::make_shared<gl_hwk::PrimitiveBuilder>(shader);

  // 渲染主程序
  auto render_func = [&]() -> void {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    gl_hwk::TextureLoader::instance().activeTexture(texture, GL_TEXTURE0);

    // 坐标变换
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    for (int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i + std::abs(glutGet(GLUT_ELAPSED_TIME) / 200.0f);
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      builder->buildTriangles(fmt::format("cube_{}", 0), vertices, {}, color,
                              model);
    }
    glFlush();
  };

  // 键盘回调
  auto keyboardCallback = [&camera](unsigned char key, int x, int y) {
    if (key == 27) {
      exit(0);
    } else if (key == 'w') {
      camera->move(camera->getFront() * 0.25f);
    } else if (key == 's') {
      camera->move(camera->getFront() * -0.25f);
    } else if (key == 'a') {
      camera->move(camera->getLeft() * 0.25f);
    } else if (key == 'd') {
      camera->move(camera->getRight() * 0.25f);
    }
  };

  // 鼠标回调
  int last_x = 0;
  int last_y = 0;
  bool first_move = true;
  auto mouseCallback = [&camera, &last_x, &last_y, &first_move](int x, int y) {
    int xoffset = x - last_x;
    int yoffset = y - last_y;

    if (first_move || std::abs(xoffset) > 100 || std::abs(yoffset) > 100) {
      last_x = x;
      last_y = y;
      first_move = false;
      return;
    }

    camera->turnYaw(xoffset * 0.05f);
    camera->turnPitch(yoffset * -0.05f);
    last_x = x;
    last_y = y;
  };

  // 注册回调
  gl_hwk::OpenGLApplication::instance().onDisplay(std::move(render_func));
  gl_hwk::OpenGLApplication::instance().onKeyboardPress(
      std::move(keyboardCallback));
  gl_hwk::OpenGLApplication::instance().onMouseMove(std::move(mouseCallback));

  // OpenGL ， 启动！
  gl_hwk::OpenGLApplication::instance().run();

  return 0;
}
