#version 330 core

layout (location = 0) in vec3 aPosition; // устанавливаем позицию аттрибута в 0
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPosition, 1.0f);
    TexCoord = aTexCoord; 
}