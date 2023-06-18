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

};