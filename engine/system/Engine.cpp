#include "Engine.hpp"

#include <iostream>

namespace ige {
Engine::Engine() {
    SDL_Init(SDL_INIT_EVERYTHING);

    std::cout << "Starting Application..."
              << "\n";

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError()
                  << std::endl;
    }

    m_window = SDL_CreateWindow("SDLGL Test", 800, 600,
                                SDL_WindowFlags::SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    m_renderer = SDL_CreateRenderer(
        m_window, NULL, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    SDL_Event event;
    bool m_running = true;
    while (m_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                m_running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                auto keysympressed = event.key.keysym.sym;
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    m_running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(m_renderer, 100, 149, 237, 255);
        SDL_RenderClear(m_renderer);
        SDL_RenderPresent(m_renderer);
    }
}
Engine::~Engine() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
}  // namespace ige