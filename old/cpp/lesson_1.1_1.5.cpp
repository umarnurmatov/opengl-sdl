#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "Shader.h"

std::string getShaderPath(std::string name)
{
    static std::string prefix(MY_SHADER_PATH);
    return prefix + name;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // using only modern functions

    SDL_Window* window = SDL_CreateWindow(
        "SDL-OpenGL", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        600, 
        800, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    int height, width;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);


    GLfloat verticies[] =
    {
         0.5f,  0.5f, 0.0f,  // Верхний правый угол
         0.5f, -0.5f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f,  // Нижний левый угол
        -0.5f,  0.5f, 0.0f   // Верхний левый угол
    };

    GLfloat verticiesTrigCol[] = 
    {
         // Позиции         // Цвета
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Нижний правый угол
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Нижний левый угол
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Верхний угол
    };

    // Vertex buffer object
    // Данные о вершинах, которые хранятся в памяти GPU
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Element buffer object
    // Даные о порядке обхода вершин
    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    // Vertex array buffer хранит в себе VBO, EBO и вызовы вершинных аттрибутов, чтоб каждый
    // раз не приходилось заново настраивать
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    Shader shader(getShaderPath("shader.vs"), getShaderPath("shader.frag"));

    // Привязываем VAO
    glBindVertexArray(VAO);

    // Копируем массив вершин в буфер OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesTrigCol), verticiesTrigCol, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Устанавливаем указатели на вершинные аттрибуты
    // Вершинные аттрибуты говорят, как интерпретировать данные из VBO
    // и как дальше передать её в шейдер
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    // Отвязываем VAO
    glBindVertexArray(0);

    SDL_Event event;
    bool quit = 0;

    while(!quit)
    {
        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // для Wireframe отрисовки
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // изменяем uniform-переменную внутри шейдера
        // GLint myColorLocation = glGetUniformLocation(shader.getShaderProgram(), "myColor");
        // glUniform4f(myColorLocation, 0.0f, 1.f, 0.5f, 1.0f);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(window); // swapping buffers
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

}