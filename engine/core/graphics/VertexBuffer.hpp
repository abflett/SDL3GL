#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace ige
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void *data, GLuint size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

    private:
        GLuint m_rendererId;
    };
}