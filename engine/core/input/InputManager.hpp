#pragma once

#include <SDL3/SDL.h>

class InputManager
{
public:
    InputManager(bool &running);

    void HandleEvents();

private:
    SDL_Event m_event;
    bool &m_running;
};