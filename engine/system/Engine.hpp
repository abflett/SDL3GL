#pragma once

#include <SDL3/SDL.h>

namespace ige {
class Engine {
   public:
    Engine();
    ~Engine();

   private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
};

}  // namespace ige
