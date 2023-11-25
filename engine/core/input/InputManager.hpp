#pragma once

#include <SDL3/SDL.h>
#include "Window.hpp"

namespace ige
{
    class InputManager
    {
    public:
        InputManager(bool &running, Window &window);

        void HandleEvents();

    private:
        SDL_Event m_event;
        bool &m_running;
        Window &m_window;
    };
}