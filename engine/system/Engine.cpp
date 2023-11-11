#include "Engine.h"

#include <SDL3/SDL.h>

#include <iostream>

namespace ige {
Engine::Engine() {
    SDL_Init(SDL_INIT_EVERYTHING);

    std::cout << "Starting Engine..."
              << "\n";
}
Engine::~Engine() { SDL_Quit(); }
}  // namespace ige