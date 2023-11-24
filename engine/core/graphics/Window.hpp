#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <memory>

namespace ige
{

    class Window
    {
    public:
        Window(int width, int height, const char *title);

        SDL_Window *GetSDLWindow() const;

    private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
    };
}