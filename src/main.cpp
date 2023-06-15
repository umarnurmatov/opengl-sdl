#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Utils/Utils.hpp"
#include "Base/Shader.hpp"
#include "MySDL/Keyboard.hpp"
#include "Camera/Camera.hpp"
#include "MySDL/SDLUtils.hpp"
#include "Render/Texture.hpp"
#include "Render/DirectLight.hpp"
#include "Render/PointLight.hpp"
#include "Render/SpotLight.hpp"
#include "Render/Mesh.hpp"

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
    
    MyGL::Shader containerShader(Utils::getShaderPath("shader.vs"), Utils::getShaderPath("shader.fs"));
    MyGL::Shader lightShader(Utils::getShaderPath("lightShader.vs"), Utils::getShaderPath("lightShader.fs"));

    GLuint containerVAO, VBO; 
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint diffuseMap = MyGL::loadTexture(Utils::getResPath("container.jpg"));
    GLuint specularMap = MyGL::loadTexture(Utils::getResPath("container2_specular.jpg"));
    containerShader.use();
    glUniform1i(containerShader.getLoc("material.diffuse"), 0);
    glUniform1i(containerShader.getLoc("material.specular"), 1);

    bool quit = false;
    SDL_Event event;

    MySDL::Keyboard keyboard;
    MyGL::Camera camera;

    DirectLight directLight;
    PointLight pointLight[NR_POINT_LIGHTS];
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        pointLight[i].setPosition(pointLightPositions[i]);
    }
    SpotLight spotLight[NR_SPOT_LIGHTS];
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        spotLight[i].setPosition({1.0f, 1.0f, 1.0f});
    }

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
        //ImGui::ShowDemoWindow();

        camera.processKeyboard(keyboard, cTime - pTime);
        for(auto& s : spotLight) { s.setPosition(camera.getPos()); s.setDirection(camera.getFront()); }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        containerShader.use();

        glm::vec3 camPos = camera.getPos();
        glUniform3fv(containerShader.getLoc("viewPos"), 1, glm::value_ptr(camPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glUniform1f(containerShader.getLoc("material.shininess"), 64.0f);

        ////////////// light
        directLight.render(containerShader);
        glUniform1i(containerShader.getLoc("NR_POINT_LIGHTS"), NR_POINT_LIGHTS);
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            std::string prefix = "pointLight[" + std::to_string(i) + "]";
            pointLight[i].render(containerShader, prefix);
        }
        glUniform1i(containerShader.getLoc("NR_SPOT_LIGHTS"), NR_SPOT_LIGHTS);
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        {
            std::string prefix = "spotLight[" + std::to_string(i) + "]";
            spotLight[i].render(containerShader, prefix);
        }

        glm::mat4 view, proj;
        view = camera.getView();
        proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(wWidth) / static_cast<float>(wHeight), 0.1f, 100.0f);

        glUniformMatrix4fv(containerShader.getLoc("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(containerShader.getLoc("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(containerVAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(containerShader.getLoc("model"), 1, GL_FALSE, glm::value_ptr(model));

            glm::mat3 norm;
            norm = glm::mat3(glm::transpose(glm::inverse(model)));
            glUniformMatrix3fv(containerShader.getLoc("norm"), 1, GL_FLOAT, glm::value_ptr(norm));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

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


