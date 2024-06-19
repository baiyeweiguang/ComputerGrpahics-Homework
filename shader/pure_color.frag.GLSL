#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}