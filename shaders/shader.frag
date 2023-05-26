#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture1; 
uniform sampler2D ourTexture2; 
uniform float scale;
void main()
{
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), scale) * vec4(ourColor, 1.f);

}