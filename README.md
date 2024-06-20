# gl_homework

> 突发奇想用库开发的形式去做课程作业

## 项目简介

**作者**: 邹承甫

gl_homework库对OpenGL（基于glut和glew）进行二次封装，实现了一系列工具类和函数，能够快速开发OpenGL应用，包括以下类：

- **OpenGLApplication** ： 单例模式，对一个OpenGL应用的抽象，快速构建一个窗口

- **Shader** ： 快速加载顶点/片段着色器代码并进行编译

- **PrimitiveBuilder** ： 构造器模式，通过*buildTriangles*、*buildLines*等函数绘制基础图元

- **TextureLoader** ： 单例模式，快速加载纹理

- **Camera** ： 创建一个摄像机


通过这个库，可以按照下面方式快速构建OpenGL应用：
```cpp
int main(int argc, char** argv) {
  // 初始化
  gl_hwk::WindowOptions options;
  options.name = argv[0];
  options.width = 1024;
  options.height = 1024;

  gl_hwk::OpenGLApplication::instance().init(argc, argv, options);

  // 着色器
  auto shader = std::make_shared<gl_hwk::Shader>(
      "vert.GLSL", "frag.GLSL");

  // 纹理
  GLuint texture =
      gl_hwk::TextureLoader::instance().loadTexture("texture/wall.jpg");
  shader->start();
  shader->setInt("texture1", 0);

  // 顶点数据
  auto vertices = std::vector<glm::vec3>{
    ...
  };
 
  // 颜色或纹理坐标
  auto colors = std::vector<glm::vec3>{     
    ...  
  };


  // 摄像机
  auto camera = std::make_shared<gl_hwk::Camera>(glm::vec3(0.0f, 0.0f, -3.0f),
                                                 600.f, 1024, 1024);

  // Builder
  auto builder = std::make_shared<gl_hwk::PrimitiveBuilder>(shader);

  // 渲染主程序
  auto render_func = [&]() -> void {
    gl_hwk::TextureLoader::instance().activeTexture(texture, GL_TEXTURE0);

    // 坐标系统
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    builder->buildTriangles(fmt::format("cube_{}", 0), vertices, {}, colors, model);
  };

  // 键盘回调
  auto keyboardCallback = [](unsigned char key, int x, int y) {
    if (key == 27) {
      exit(0);
    } 
  };


  // 注册回调
  gl_hwk::OpenGLApplication::instance().onDisplay(std::move(render_func));
  gl_hwk::OpenGLApplication::instance().onKeyboardPress(
      std::move(keyboardCallback));

  // OpenGL ， 启动！
  gl_hwk::OpenGLApplication::instance().run();

  return 0;
}
```

## 依赖

- **Ubuntu22.04** ： 本项目只在Ubuntu22.04下进行过测试

- **Xmake** ： 本项目使用xmke进行构建
    ```baseh
    wget https://xmake.io/shget.text -O - | bash
    ```
- **OpenGL**

- **freeglut**

- **OpenCV**
   ```bash
   sudo apt install libopencv-dev
   ```

- **fmt**
  ```bash
  sudo apt install libfmt-dev
  ```

## 编译和运行

- 编译
    ```
    xmake
    ```

- 运行
    ```
    xmake run main
    ```

- 生成complie_commands.json文件用于clangd提示生成
    ```
    xmake project -k complie_commands
    ```