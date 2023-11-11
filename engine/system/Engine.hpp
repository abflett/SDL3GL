#pragma once

#include <SDL3/SDL.h>
// #include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl.h>
#include <gl/GL.h>

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
