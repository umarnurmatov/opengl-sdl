#include "Keyboard.hpp"

namespace MySDL
{

void Keyboard::pollEvent(SDL_Event &event)
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

bool Keyboard::isKeyPressed(SDL_Keycode keycode) 
{ 
    if(m_keys[keycode]) return true; 
    return false; 
}

};