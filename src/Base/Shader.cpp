#include "Shader.hpp"


namespace Engine
{

Shader::Shader(std::string vpath, std::string fpath)
{
    GLuint vertexShader = loadShader(vpath, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fpath, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        logCritical(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::use()
{
    glUseProgram(program);
}

GLint Shader::getLoc(std::string name)
{
    return glGetUniformLocation(program, name.c_str());
}

GLuint& Shader::getProgram()
{
    return program;
}

GLuint Shader::loadShader(std::string &path, GLenum type)
{
    GLuint shader = glCreateShader(type);
    std::string shaderSource;
    readFile(path, shaderSource);
    const GLchar* shaderSourceGL = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceGL, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        logCritical(infoLog);
    }

    return shader;
}



void Shader::readFile(std::string path, std::string &s)
{
    std::ostringstream ss;
    std::ifstream file;
    file.open(path);
    if(file.is_open())
    {
        ss << file.rdbuf();
        file.close();
    }
    else
    {
        logCritical("Open file");
    }
    s = ss.str();
}

};