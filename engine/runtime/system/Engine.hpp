#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>

#include "Window.hpp"

namespace ige
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

    private:
        Window m_window;
        SDL_GLContext m_glContext;
        bool m_running;
        float aspectRatio;
        glm::mat4 m_mvp;
        GLfloat m_orthoSize;
    };
}
