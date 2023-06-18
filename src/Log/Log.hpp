#pragma once
#include <SDL2/SDL_log.h>
#include <glad/glad.h>

#include <source_location>
#include <string>
#include <format>


namespace Engine
{

void logCritical(std::string msg, std::source_location loc = std::source_location::current());
void logError(std::string msg, std::source_location loc = std::source_location::current());
void logInfo(std::string msg, std::source_location loc = std::source_location::current());

};