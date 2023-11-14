#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include <memory>

#include "VertexBuffer.hpp"

namespace ige
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

    private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
        SDL_GLContext m_glContext;
    };

} // namespace ige
