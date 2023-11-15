#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

#include <glad/glad.h>

namespace ige
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
        void Bind() const;
        void Unbind() const;

    private:
        GLuint m_rendererId;
    };
}