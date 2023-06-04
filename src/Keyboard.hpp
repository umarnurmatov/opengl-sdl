#pragma once
#include <SDL2/SDL.h>

#include <unordered_map>

namespace MySDL
{
class Keyboard
{
public:
    void pollEvent(SDL_Event &event)
    {
        switch(event.type) 
        {
        case SDL_KEYDOWN:
            m_keys[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            m_keys[event.key.keysym.sym] = false;
            break;
        }
    }

    bool isKeyPressed(SDL_Keycode keycode) 
    { 
        if(m_keys[keycode]) return true; 
        return false; 
    }

private:
    std::unordered_map<SDL_Keycode, bool> m_keys;
};
};