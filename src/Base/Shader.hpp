#pragma once
#include <glad/glad.h>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "Log.hpp"

namespace Engine
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
    GLuint program;

    GLuint loadShader(std::string &path, GLenum type);

    void readFile(std::string path, std::string &s);
};

};