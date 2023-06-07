#pragma once
#include <SDL2/SDL.h>

#include <unordered_map>

namespace MySDL
{
class Keyboard
{
public:
    void pollEvent(SDL_Event &event);
    
    bool isKeyPressed(SDL_Keycode keycode);

private:
    std::unordered_map<SDL_Keycode, bool> m_keys;
};
};