#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture1; 
uniform sampler2D ourTexture2; 

void main()
{
    fragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2f);
}