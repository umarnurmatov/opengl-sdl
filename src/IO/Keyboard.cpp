#include "Keyboard.hpp"

namespace Engine
{

void Keyboard::pollEvent(SDL_Event &event)
{
    switch(event.type) 
    {
    case SDL_KEYDOWN:
        keys[event.key.keysym.sym] = true;
        break;
    case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        break;
    }
}

bool Keyboard::isKeyPressed(SDL_Keycode keycode) 
{ 
    if(keys[keycode]) return true; 
    return false; 
}

};