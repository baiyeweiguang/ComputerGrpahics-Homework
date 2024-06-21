#version 330 core
out vec4 FragColor;

in vec3 light; 
in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, TexCoords); 
    vec3 objectColor = textureColor.rgb;
    vec3 color = light * objectColor;

    FragColor = vec4(color, textureColor.a);
}