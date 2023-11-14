#include "ElementBuffer.hpp"

#include <iostream>

namespace ige
{
    ElementBuffer::ElementBuffer(const GLuint *data, GLuint count)
        : m_count(count)
    {
        std::cout << "Creating ElementBuffer..."
                  << "\n";

        glGenBuffers(1, &m_rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, GL_STATIC_DRAW);
    }

    ElementBuffer::~ElementBuffer()
    {
        glDeleteBuffers(1, &m_rendererId);
    }

    void ElementBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    }

    void ElementBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint ElementBuffer::GetCount() const
    {
        return m_count;
    }
}