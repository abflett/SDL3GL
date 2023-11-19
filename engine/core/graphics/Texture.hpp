#pragma once

#include <glad/glad.h>
#include <string>

namespace ige
{
    class Texture
    {
    public:
        Texture(const std::string path);
        ~Texture();

        void Bind(GLuint slot = 0) const;
        void Unbind() const;

        GLuint GetWidth() const;
        GLuint GetHeight() const;

    private:
        GLuint m_rendererId;
        std::string m_filePath;
        GLuint m_width;
        GLuint m_height;
    };
}