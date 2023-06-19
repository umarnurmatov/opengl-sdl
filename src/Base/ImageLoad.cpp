#include "ImageLoad.hpp"

namespace Engine
{

void flipSurfaceVertical(SDL_Surface* surface)
{
    SDL_LockSurface(surface);

    // SDL_PixelFormat* pf = surface->format;
    // if(surface->format->BitsPerPixel != 8)
    // {
    //     std::cerr << std::source_location::current().function_name() << ": not an 8 bit image!" << std::endl;
    //     return;
    // }

    char* pixels = (char*)surface->pixels;
    int pitch = surface->pitch;
    char buffer[pitch];


    for(int i = 0; i < static_cast<int>(surface->h/2); i++)
    {
        char* row1 = pixels + i*pitch;
        char* row2 = pixels + (surface->h - i - 1)*pitch;

        SDL_memcpy(buffer, row1, pitch);
        SDL_memcpy(row1, row2, pitch);
        SDL_memcpy(row2, buffer, pitch);
    }

    SDL_UnlockSurface(surface);
}

SDL_Surface* loadImage(const char* path)
{
    // load sample.png in to image
    SDL_Surface *image;
    image=IMG_Load_RW(SDL_RWFromFile(path, "rb"), 1);
    if(!image)
        logCritical(IMG_GetError());

    logInfo(SDL_GetPixelFormatName(image->format->format));
    logInfoFmt("Bytes per pixel: {}", static_cast<unsigned int>(image->format->BytesPerPixel));
    return image;
}

};