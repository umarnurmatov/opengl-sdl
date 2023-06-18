#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Log.hpp"


namespace Engine
{

void flipSurfaceVertical(SDL_Surface* surface);
SDL_Surface* loadImage(const char* path);

};