#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <source_location>
#include <string>
#include <format>


namespace Engine
{

void logCritical(std::string msg, std::source_location loc = std::source_location::current());
void logError(std::string msg, std::source_location loc = std::source_location::current());
void logInfo(std::string msg, std::source_location loc = std::source_location::current());

template<typename... StringArgs>
void logCriticalFmt(std::string_view fmt, StringArgs&&... args)
{
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s\n", std::vformat(fmt, std::make_format_args(args...)).c_str());
}
template<typename... StringArgs>
void logErrorFmt(std::string_view fmt, StringArgs&&... args)
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", std::vformat(fmt, std::make_format_args(args...)).c_str());
}
template<typename... StringArgs>
void logInfoFmt(std::string_view fmt, StringArgs&&... args)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s\n", std::vformat(fmt, std::make_format_args(args...)).c_str());
}

void GLDebugInit();

void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam);

};