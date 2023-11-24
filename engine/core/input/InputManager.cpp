#include "InputManager.hpp"

InputManager::InputManager(bool &running) : m_running(running)
{
}

void InputManager::HandleEvents()
{
    while (SDL_PollEvent(&m_event))
    {
        // ImGui_ImplSDL3_ProcessEvent(&event);
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
            // aspectRatio = static_cast<GLfloat>(event.window.data1) / static_cast<GLfloat>(event.window.data2);
            // std::cout << "Aspect Ration Changed: " << aspectRatio << std::endl;

            // GLfloat orthoSizeX = m_orthoSize * aspectRatio;
            // m_mvp = glm::ortho(-orthoSizeX, orthoSizeX, -m_orthoSize, m_orthoSize, -1.0f, 200.0f);
        }
    }
}