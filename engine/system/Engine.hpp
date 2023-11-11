#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace ige {
class Engine {
   public:
    Engine();
    ~Engine();

   private:
    SDL_Window *m_window;
    SDL_GLContext m_glContext;
};

}  // namespace ige
