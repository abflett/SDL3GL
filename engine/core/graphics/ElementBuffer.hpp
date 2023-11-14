#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace ige
{
    class ElementBuffer
    {
    public:
        ElementBuffer(const GLuint *data, GLuint count);
        ~ElementBuffer();

        void Bind() const;
        void Unbind() const;

        GLuint GetCount() const;

    private:
        GLuint m_rendererId;
        GLuint m_count;
    };
}