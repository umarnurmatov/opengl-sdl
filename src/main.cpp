#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Filesystem/Filesystem.hpp"
#include "Base/Shader.hpp"
#include "IO/Keyboard.hpp"
#include "Camera/Camera.hpp"
#include "Render/DirectLight.hpp"
#include "Render/PointLight.hpp"
#include "Render/SpotLight.hpp"
#include "Render/Mesh.hpp"
#include "Render/Model.hpp"

#define NR_POINT_LIGHTS 4 // see also shader.fs for MAX_NR_POINT_LIGHTS
#define NR_SPOT_LIGHTS 1 // see also shader.fs

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = 
{
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

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};  

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Engine::logCritical(SDL_GetError());
    }

    int SDL_Image_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int SDL_Image_init  = IMG_Init(SDL_Image_flags);
    if((SDL_Image_init & SDL_Image_flags) != SDL_Image_flags) 
    {
        Engine::logCritical(IMG_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(
        "Lightning",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // using only modern functions

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress));

    Engine::GLDebugInit();

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
    
    Engine::Shader modelShader(Engine::getShaderPath("shader.vs"), Engine::getShaderPath("shader.fs"));
    Engine::Shader lightShader(Engine::getShaderPath("lightShader.vs"), Engine::getShaderPath("lightShader.fs"));

    GLuint lightVAO, VBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
   
    bool quit = false;
    SDL_Event event;

    Engine::Keyboard keyboard;
    Engine::Camera camera;

    Engine::DirectLight directLight;
    Engine::PointLight pointLight[NR_POINT_LIGHTS];
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        pointLight[i].setPosition(pointLightPositions[i]);
    }
    Engine::SpotLight spotLight[NR_SPOT_LIGHTS];
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        spotLight[i].setPosition({1.0f, 1.0f, 1.0f});
    }

    Engine::Model backpackModel(Engine::getResPath("backpack/backpack.obj"));


    float cTime = SDL_GetTicks64();
    float pTime = cTime;

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
                if(io.WantCaptureMouse || !SDL_GetRelativeMouseMode())
                   break; 
                camera.processMouseMove(event.motion.xrel, event.motion.yrel);
                break;
            case SDL_MOUSEWHEEL:
                if(!SDL_GetRelativeMouseMode()) 
                    break;
                camera.processMouseWheel(event.wheel.y);
                break;
            }

            keyboard.pollEvent(event);
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow();

        camera.processKeyboard(keyboard, cTime - pTime);
        for(auto& s : spotLight) { s.setPosition(camera.getPos()); s.setDirection(camera.getFront()); }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.use();

        glm::vec3 camPos = camera.getPos();
        glUniform3fv(modelShader.getLoc("viewPos"), 1, glm::value_ptr(camPos));

        
        ////////////// light
        directLight.render(modelShader);
        glUniform1i(modelShader.getLoc("NR_POINT_LIGHTS"), NR_POINT_LIGHTS);
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            std::string prefix = "pointLight[" + std::to_string(i) + "]";
            pointLight[i].render(modelShader, prefix);
        }
        glUniform1i(modelShader.getLoc("NR_SPOT_LIGHTS"), NR_SPOT_LIGHTS);
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        {
            std::string prefix = "spotLight[" + std::to_string(i) + "]";
            spotLight[i].render(modelShader, prefix);
        }

        glm::mat4 view, proj;
        view = camera.getView();
        proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(wWidth) / static_cast<float>(wHeight), 0.1f, 100.0f);

        glUniformMatrix4fv(modelShader.getLoc("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelShader.getLoc("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glm::mat4 model(1.);
        glUniformMatrix4fv(modelShader.getLoc("model"), 1, GL_FALSE, glm::value_ptr(model));

        glm::mat3 norm;
        norm = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(modelShader.getLoc("norm"), 1, GL_FLOAT, glm::value_ptr(norm));

        backpackModel.draw(modelShader);

        lightShader.use();

        glUniformMatrix4fv(lightShader.getLoc("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lightShader.getLoc("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(lightVAO);
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            glm::mat4 model_light(1.0f);
            model_light = glm::translate(model_light, pointLight[i].getPosition());
            model_light = glm::scale(model_light, glm::vec3(0.2f));
            glUniformMatrix4fv(lightShader.getLoc("model"), 1, GL_FALSE, glm::value_ptr(model_light));

            glUniform3fv(lightShader.getLoc("lightColor"), 1, glm::value_ptr(pointLight[i].getColor()));

            glDrawArrays(GL_TRIANGLES, 0, 36); 
        }
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


