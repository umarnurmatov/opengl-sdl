#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Utils.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include "Camera.hpp"
#include "MySDL.hpp"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main()
{
    MySDL::initSDL(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Lightning",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    MySDL::initSDL_Image();

    MySDL::setGLAttributes();
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress));

    //// IMGUI ////
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ///////////////

    int wHeight, wWidth;
    SDL_GetWindowSize(window, &wWidth, &wHeight);
    glViewport(0, 0, wWidth, wHeight);

    SDL_SetRelativeMouseMode(SDL_TRUE);
    int mousePosX, mousePosY;
    SDL_GetMouseState(&mousePosX, &mousePosY);
    SDL_WarpMouseInWindow(window, mousePosX, mousePosY); 

    glEnable(GL_DEPTH_TEST);
    
    MyGL::Shader containerShader(Utils::getShaderPath("shader_lesson_2.1.vs"), Utils::getShaderPath("shader_lesson_2.1.fs"));
    MyGL::Shader lightShader(Utils::getShaderPath("lightShader_lesson_2.1.vs"), Utils::getShaderPath("lightShader_lesson_2.1.fs"));

    GLuint containerVAO, VBO; 
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    bool quit = false;
    SDL_Event event;

    MySDL::Keyboard keyboard;
    MyGL::Camera camera;

    float cTime = SDL_GetTicks64();
    float pTime = cTime;

    glm::vec3 lightPos(1.2f, 0.0f, 2.0f);
    bool first = true;
    while(!quit)
    {
        cTime = SDL_GetTicks64();

        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
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
                case SDLK_i:
                    if(SDL_GetRelativeMouseMode()) SDL_SetRelativeMouseMode(SDL_FALSE);
                    else SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                if(first) 
                {
                    first = false;
                    break;
                }
                if(io.WantCaptureMouse)
                   break; 
                 
                camera.processMouseMove(event.motion.xrel, event.motion.yrel);
                break;
            case SDL_MOUSEWHEEL:
                camera.processMouseWheel(event.wheel.y);
                break;
            }

            keyboard.pollEvent(event);
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        camera.processKeyboard(keyboard, cTime - pTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f), view, proj;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
        view = camera.getView();
        proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<GLfloat>(wWidth / wHeight), 0.1f, 100.0f);

        glm::mat3 norm;
        norm = glm::mat3(glm::transpose(glm::inverse(model)));

        glm::vec3 camPos = camera.getPos();

        containerShader.use();

        glUniformMatrix4fv(containerShader.getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(containerShader.getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(containerShader.getLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix3fv(containerShader.getLocation("norm"), 1, GL_FLOAT, glm::value_ptr(norm));

        glUniform3f(containerShader.getLocation("objColor"), 1.0f, 0.5f, 0.31f);
        glUniform3f(containerShader.getLocation("lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(containerShader.getLocation("lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3fv(containerShader.getLocation("viewPos"), 1, glm::value_ptr(camPos));

        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        glBindVertexArray(0);

        lightShader.use();

        glm::mat4 model_light(1.0f);
        model_light = glm::translate(model_light, lightPos);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        glUniformMatrix4fv(lightShader.getLocation("model"), 1, GL_FALSE, glm::value_ptr(model_light));
        glUniformMatrix4fv(lightShader.getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightShader.getLocation("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        glBindVertexArray(0);

        pTime = cTime;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit(); 

    return 0;
}


