#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace MySDL
{
void initSDL(unsigned int flags);
void initSDL_Image();
void flipSurfaceVertical(SDL_Surface* surface);
SDL_Surface* loadImage(const char* path);
void setGLAttributes();

};