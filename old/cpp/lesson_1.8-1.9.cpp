#include <unordered_map>
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
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

    std::cout << SDL_SetRelativeMouseMode(SDL_TRUE);

    MySDL::initSDL_Image();

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST); // Z-буфер

    int height, width;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    MyGL::Shader shader(Utils::getShaderPath("shader_lesson_1.8.vs"), Utils::getShaderPath("shader_lesson_1.8.fs"));

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    SDL_Surface* image2 = MySDL::loadImage(Utils::getResPath("img3.jpg").c_str());
    MySDL::flipSurfaceVertical(image2);

    GLuint tex;
    glGenTextures(1, &tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image2->w, image2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_Event event;
    bool quit = 0;

    MyGL::Camera camera;
    MySDL::Keyboard keyboard;
    
    auto pTime = SDL_GetTicks64();
    auto cTime = pTime;
    while(!quit)
    {
        cTime = SDL_GetTicks64();
        while(SDL_PollEvent(&event))
        {
            switch(event.type) 
            {
            case SDL_QUIT:
                quit = true;
                break; 
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_q:
                    if(SDL_GetModState() & KMOD_CTRL) quit = true;
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                camera.processMouseMove(event.motion.xrel, event.motion.yrel);
                break;
            }

            keyboard.pollEvent(event);
        }

        camera.processKeyboard(keyboard, cTime - pTime);

        glClearColor(0.65098f, 0.062745f, 0.117647f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view;
        view = camera.getView(); 
    
        glm::mat4 proj;
        proj = glm::perspective(glm::radians(45.f), static_cast<float>(width / height), 0.1f, 100.0f);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(glGetUniformLocation(shader.getProgram(), "tex"), 0);

        shader.use();

        glBindVertexArray(VAO);
        for(GLuint i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(static_cast<float>(SDL_GetTicks64()) / 90.f), glm::vec3(1.0f, 0.2f, 0.3f));
   
            glUniformMatrix4fv(shader.getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        glUniformMatrix4fv(shader.getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shader.getLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));
 
           
        SDL_GL_SwapWindow(window); // swapping buffers

        pTime = cTime;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();

}