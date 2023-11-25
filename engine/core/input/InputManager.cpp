#include "InputManager.hpp"

#include <imgui_impl_sdl3.h>

namespace ige
{
    InputManager::InputManager(bool &running, Window &window) : m_running(running), m_window(window)
    {
    }

    void InputManager::HandleEvents()
    {
        while (SDL_PollEvent(&m_event))
        {
            ImGui_ImplSDL3_ProcessEvent(&m_event);
            if (m_event.type == SDL_EVENT_QUIT)
            {
                m_running = false;
            }
            if (m_event.type == SDL_EVENT_KEY_DOWN)
            {
                auto keysympressed = m_event.key.keysym.sym;
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_running = false;
                }
            }
            if (m_event.type == SDL_EVENT_WINDOW_RESIZED &&
                m_event.window.type == SDL_EVENT_WINDOW_RESIZED)
            {
                m_window.UpdateAspectRatio();
            }
        }
    }
}