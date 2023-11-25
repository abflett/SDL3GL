#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <iostream>
#include <memory>

namespace ige
{

    class Window
    {
    public:
        Window(int width, int height, const char *title);

        SDL_Window *GetSDLWindow() const;
        glm::vec2 GetDimensions() const;
        float GetAspectRatio() const;
        void UpdateAspectRatio();

    private:
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
        float m_aspectRatio;
    };
}