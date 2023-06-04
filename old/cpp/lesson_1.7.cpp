#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"
#include "MySDL.hpp"
#include "Utils.hpp"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    MySDL::setGLAttributes();

    SDL_Window* window = SDL_CreateWindow(
        "SDL-OpenGL", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        1920, 
        1080, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    MySDL::initSDL_Image();

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    int height, width;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Shader shader(Utils::getShaderPath("shader_lesson_1.7.vs"), Utils::getShaderPath("shader_lesson_1.7.frag"));

    GLfloat verticies[] =
    {
          // Позиции        Текстурные координаты
         0.5f,  0.5f, 0.0f, 4.0f, 4.0f,   // Верхний правый
         0.5f, -0.5f, 0.0f, 4.0f, 0.0f,   // Нижний правый
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // Нижний левый
        -0.5f,  0.5f, 0.0f, 0.0f, 4.0f    // Верхний левый   
    };
    GLuint indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Привязываем VAO
    glBindVertexArray(VAO);

    // Копируем массив вершин в буфер OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Устанавливаем указатели на вершинные аттрибуты
    // Вершинные аттрибуты говорят, как интерпретировать данные из VBO
    // и как дальше передать её в шейдер

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Отвязываем VAO
    glBindVertexArray(0);


    // ТЕКСТУРА
    SDL_Surface* image1 = MySDL::loadImage(Utils::getResPath("img2.jpg").c_str());
    SDL_Surface* image2 = MySDL::loadImage(Utils::getResPath("img.jpg").c_str());
    MySDL::flipSurfaceVertical(image1);
    MySDL::flipSurfaceVertical(image2);


    ////////////
    // ТЕКСТУРА 1
    ////////////
    GLuint texture1;
    glGenTextures(1, &texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image1->w, image1->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(image1);
    glBindTexture(GL_TEXTURE_2D, 0);

    ////////////
    // ТЕКСТУРА 2 
    ////////////
    GLuint texture2;
    glGenTextures(1, &texture2);

    // установка поведения, если текстурные координаты выходят за пределы (0,0) (1,1)
    // s, t, r = x, y, z
    // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TP_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // мипмап - набор изображений-текстур, где каждое следующее вдвое меньше другого
    // GL_NEARESET_MIPMAP_NEAREST: Выбирает ближайший мипмап, соотносящийся с размером пикселя и также используется интерполяция ближайшего соседа для сэмплинга текстур.
    // GL_LINEAR_MIPMAP_NEAREST: Выбирает ближайший мипмап и сэмплирует его методом линейной интерполяции.
    // GL_NEAREST_MIPMAP_LINEAR: Линейная интерполяция между двумя ближайшими мипмапами и сэмплирование текстур с помощью линейной интерполяции.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image2->w, image2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_Event event;
    bool quit = 0;
    GLfloat scale = 0.0f;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if(scale <= 1.f) scale += 0.1f;
                    else scale = 0.f;
                    break;
                case SDLK_DOWN:
                    if(scale >= 0.f) scale -= 0.1f;
                    else scale = 1.0f;
                    break;
                }
            }
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "ourTexture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "ourTexture2"), 1);


        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, glm::radians(static_cast<float>(SDL_GetTicks64()) / 90.f), glm::vec3(0.0f, 0.0f, 1.0f));

        shader.use();
        GLint transformLoc = glGetUniformLocation(shader.getShaderProgram(), "trans");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
 
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sizeof(indices) / sizeof(GLuint)), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window); // swapping buffers
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();

}