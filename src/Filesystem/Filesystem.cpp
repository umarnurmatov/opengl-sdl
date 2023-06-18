#include "Filesystem.hpp"

namespace Engine
{

std::string getShaderPath(std::string name)
{
    static std::string prefix(MY_SHADER_PATH);
    return prefix + name;
}

std::string getResPath(std::string name)
{
    static std::string prefix(MY_RESOURCE_PATH);
    return prefix + name;
}

};