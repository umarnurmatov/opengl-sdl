#include "TextureLoad.hpp"

namespace Engine
{

GLuint loadTexture(std::string path)
{
    SDL_Surface* image2 = loadImage(path.c_str());
    flipSurfaceVertical(image2);

    GLuint tex;
    glGenTextures(1, &tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image2->w, image2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

};