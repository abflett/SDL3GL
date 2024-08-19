#include "Window.hpp"

namespace ige
{

    Window::Window(int width, int height, const char *title)
        : m_window(SDL_CreateWindow(title, width, height,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN),
                   &SDL_DestroyWindow),
          m_aspectRatio(static_cast<float>(width) / static_cast<float>(height))
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError()
                      << std::endl;
        }

        // SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef _DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

        if (!m_window)
        {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        }

        UpdateAspectRatio();
    }

    SDL_Window *Window::GetSDLWindow() const
    {
        return m_window.get();
    }

    glm::vec2 Window::GetDimensions() const
    {
        int width;
        int height;
        int getSizeResult = SDL_GetWindowSize(m_window.get(), &width, &height);
        if (getSizeResult < 0)
        {
            std::cerr << "Failed to get window size: " << SDL_GetError() << std::endl;
        }

        return glm::vec2(width, height);
    }

    float Window::GetAspectRatio() const
    {
        return m_aspectRatio;
    }

    void Window::UpdateAspectRatio()
    {
        glm::vec2 windowDimensions = GetDimensions();
        m_aspectRatio = windowDimensions.x / windowDimensions.y;
    }
}