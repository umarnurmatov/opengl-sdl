#pragma once
#include <glad/glad.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

namespace MyGL
{
class Shader
{
public:
    // vpath - путь к вершинному шейдеру, fpath - к фрагментному
    Shader(std::string vpath, std::string fpath)
    {
        GLuint vertexShader = m_loadShader(vpath, GL_VERTEX_SHADER);
        GLuint fragmentShader = m_loadShader(fpath, GL_FRAGMENT_SHADER);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);

        GLint success;
        glGetProgramiv(m_program, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(m_program, 512, NULL, infoLog);
            std::cerr << "Error program linking" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

    void use()
    {
        glUseProgram(m_program);
    }

    GLint getLocation(std::string name)
    {
        return glGetUniformLocation(m_program, name.c_str());
    }

    GLuint& getProgram()
    {
        return m_program;
    }

private:
    GLuint m_program;

    GLuint m_loadShader(std::string &path, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        std::string shaderSource;
        m_readFile(path, shaderSource);
        const GLchar* shaderSourceGL = shaderSource.c_str();
        glShaderSource(shader, 1, &shaderSourceGL, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Error shader compilation " << infoLog << std::endl;
        }

        return shader;
    }



    void m_readFile(std::string path, std::string &s)
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
            std::cerr << "Erorr read file " << path << std::endl;
        }
        s = ss.str();
    }
};
};