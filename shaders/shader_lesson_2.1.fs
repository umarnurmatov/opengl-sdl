#version 330 core
out vec4 color;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 pos;
    vec3 ambient; // цвет, неяркий
    vec3 diffuse; // цвет
    vec3 specular;
};
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular;

    vec3 light = (ambient + diffuse + specular);

    color = vec4(light, 1.0f);
}