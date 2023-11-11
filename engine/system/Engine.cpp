#include "Engine.hpp"

#include <iostream>

namespace ige {
Engine::Engine() {
    SDL_Init(SDL_INIT_EVERYTHING);

    std::cout << "Starting Engine..."
              << "\n";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError()
                  << std::endl;
    }

    m_window = SDL_CreateWindow("SDLGL Test", 800, 600,
                                SDL_WindowFlags::SDL_WINDOW_OPENGL |
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

    auto m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        // Handle OpenGL context creation failure
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

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Your rendering code here

        SDL_GL_SwapWindow(m_window);

        // SDL_SetRenderDrawColor(m_renderer, 100, 149, 237, 255);
        // SDL_RenderClear(m_renderer);
        // SDL_RenderPresent(m_renderer);
    }

    SDL_GL_DeleteContext(m_glContext);
}
Engine::~Engine() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
}  // namespace ige