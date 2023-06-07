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
    Shader(std::string vpath, std::string fpath);
    
    void use();

    GLint getLoc(std::string name);

    GLuint& getProgram();

private:
    GLuint m_program;

    GLuint m_loadShader(std::string &path, GLenum type);

    void m_readFile(std::string path, std::string &s);
};
};