#include "Log.hpp"

namespace Engine
{

void logCritical(std::string msg, std::source_location loc)
{
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s: %s\n", loc.function_name(), msg.c_str());
}

void logError(std::string msg, std::source_location loc)
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: %s\n", loc.function_name(), msg.c_str());
}

void logInfo(std::string msg, std::source_location loc)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s: %s\n", loc.function_name(), msg.c_str());
}

void GLDebugInit()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    //Allow for synchronous callbacks.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
 
    //Set up the debug info callback
    glDebugMessageCallback((GLDEBUGPROC)&GLDebugCallback, NULL);
 
    //Set up the type of debug information we want to receive
    uint32_t uiUnusedIDs = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &uiUnusedIDs, GL_TRUE); //Enable all
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); //Disable notifications
}

void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam)
{
    static const char * severityEnum[] = {"High", "Medium", "Low", "Notification"};
    static const char * typeEnum[]     = {"Error", "Deprecated", "Undefined", "Portability", "Performance", "Other"};
    static const char * sourceEnum[]   = {"OpenGL", "OS", "GLSL Compiler", "3rd Party", "Application", "Other"};


    uint32_t severityID;
    switch (severity) 
    {
    case GL_DEBUG_SEVERITY_HIGH:
        severityID = 0; break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severityID = 1; break;
    case GL_DEBUG_SEVERITY_LOW:
        severityID = 2; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    default:
        severityID = 3; break;
    }

    uint32_t typeID;
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        typeID = 0; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeID = 1; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeID = 2; break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeID = 3; break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeID = 4; break;
    case GL_DEBUG_TYPE_OTHER:
    default:
        typeID = 5; break;
    }

    uint32_t sourceID;
    switch (source) 
    {
    case GL_DEBUG_SOURCE_API:
        sourceID = 0; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceID = 1; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceID = 2; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceID = 3; break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceID = 4; break;
    case GL_DEBUG_SOURCE_OTHER:
    default:
        sourceID = 5; break;
    }

    SDL_LogCritical
    (
        SDL_LOG_CATEGORY_APPLICATION, 
        "OpenGL Debug: Severity=%s, Type=%s, Source=%s - %s", 
        severityEnum[severityID], 
        typeEnum[typeID], 
        sourceEnum[sourceID], 
        message
    );
}


};