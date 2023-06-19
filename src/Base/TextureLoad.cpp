#include "TextureLoad.hpp"

namespace Engine
{

GLuint loadTexture(std::string path)
{
    SDL_Surface* image = loadImage(path.c_str());
    flipSurfaceVertical(image);

    unsigned int bytesPerPixel = static_cast<unsigned int>(image->format->BytesPerPixel);

    GLenum pixelFormat;
    GLenum internalPixelFormat;
    switch(bytesPerPixel)
    {
    case 1:
        internalPixelFormat = GL_RED;
        pixelFormat = GL_RED;
        break;
    case 3:
        internalPixelFormat = GL_RGB;
        if(image->format->Rmask == 0x000000ff)
            pixelFormat = GL_RGB;
        else
            pixelFormat = GL_BGR;
        break;
    case 4:
        internalPixelFormat = GL_RGBA;
        if(image->format->Rmask == 0x000000ff)
            pixelFormat = GL_RGBA;
        else
            pixelFormat = GL_BGRA;
        break;
    default:
        logCritical("Not supported pixel format");
        break;
    }

    GLuint tex;
    glGenTextures(1, &tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, internalPixelFormat, image->w, image->h, 0, pixelFormat, GL_UNSIGNED_BYTE, image->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

};