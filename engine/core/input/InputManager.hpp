#pragma once

#include <SDL3/SDL.h>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void HandleEvents(SDL_Event &event);

private:
};