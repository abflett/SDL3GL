#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <memory>

namespace ige
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        void HandleEvents(SDL_Event &event);

    private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
        SDL_GLContext m_glContext;
        bool m_running;
        float aspectRatio;
    };
}
