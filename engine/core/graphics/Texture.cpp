#include "Texture.hpp"

#include <SDL3_image/SDL_image.h>
#include <iostream>

namespace ige
{
    Texture::Texture(const std::string path) : m_filePath(path),
                                               m_width(0),
                                               m_height(0)
    {
        SDL_Surface *surface = IMG_Load(path.c_str());
        if (!surface)
        {
            std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        }

        m_width = surface->w;
        m_height = surface->h;

        glGenTextures(1, &m_rendererId);
        glBindTexture(GL_TEXTURE_2D, m_rendererId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        Bind();
        SDL_DestroySurface(surface);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_rendererId);
    }

    void Texture::Bind(GLuint slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_rendererId);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture::GetWidth() const
    {
        return GLuint();
    }

    GLuint Texture::GetHeight() const
    {
        return GLuint();
    }
}