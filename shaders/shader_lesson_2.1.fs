#version 330 core

#define MAX_NR_POINT_LIGHTS 64
#define MAX_NR_SPOT_LIGHTS  64

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct PointLight
{
    vec3 pos;

    vec3 ambient; // цвет, фоновый
    vec3 diffuse; // цвет
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};
uniform int NR_POINT_LIGHTS;
uniform PointLight pointLight[MAX_NR_POINT_LIGHTS];

struct DirectLight
{
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectLight directLight;

struct SpotLight
{
    vec3 pos;
    vec3 dir;

    vec3 ambient; // цвет, фоновый
    vec3 diffuse; // цвет
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;

    float cutOffIn;
    float cutOffOut;
};
uniform int NR_SPOT_LIGHTS;
uniform SpotLight spotLight[MAX_NR_SPOT_LIGHTS];


vec3 calcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 light = calcDirectLight(directLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        light += calcPointLight(pointLight[i], norm, FragPos, viewDir);
    }
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        light += calcSpotLight(spotLight[i], norm, FragPos, viewDir);
    }
    color = vec4(light, 1.0f);
}

vec3 calcDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.dir);
    float diff = max(dot(normal, lightDir), 0.0f);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)); 

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float dist = length(light.pos - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.linear * (dist*dist));
    
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)); 

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0f);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float dist = length(light.pos - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.linear * (dist*dist));

    float theta = dot(lightDir, normalize(-light.dir));
    float eps = light.cutOffIn - light.cutOffOut;
    float intensity = clamp((theta - light.cutOffOut) / eps, 0.0f, 1.0f);
    
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse * diff * intensity * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * intensity * vec3(texture(material.specular, TexCoord)); 

    return (ambient + diffuse + specular) * attenuation;
}