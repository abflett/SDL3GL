#pragma once

#include <vector>
#include <glad/glad.h>

#include "VertexBufferElement.hpp"

namespace ige
{
    class VertexBufferLayout
    {
    public:
        VertexBufferLayout();

        template <typename T>
        void Push(GLuint count);

        std::vector<VertexBufferElement> GetElements() const;

        GLuint GetStride() const;

    private:
        std::vector<VertexBufferElement> m_elements;
        GLuint m_stride;
    };
}