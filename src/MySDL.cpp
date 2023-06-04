#include "MySDL.hpp"

namespace MySDL
{
void initSDL_Image()
{
    // load support for the JPG and PNG image formats
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if((initted&flags) != flags) 
    {
        std::cerr << "IMG_Init: Failed to init required jpg and png support!\\n";
        std::cerr << "IMG_Init: \\n" <<  IMG_GetError() << std::endl;
    }
}

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
    {
        std::cerr << "IMG_Load_RW: \\n" << IMG_GetError() << std::endl;
        // handle error
    }
    return image;
}

void setGLAttributes()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // using only modern functions
}
}