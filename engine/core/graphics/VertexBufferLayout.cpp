#include "VertexBufferLayout.hpp"

namespace ige
{
    VertexBufferLayout::VertexBufferLayout() : m_stride(0) {}

    // Define the template specialization for Push<GLfloat>
    template <>
    void VertexBufferLayout::Push<GLfloat>(GLuint count)
    {
        m_elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    // Define the template specialization for Push<GLuint>
    template <>
    void VertexBufferLayout::Push<GLuint>(GLuint count)
    {
        m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    // Define the template specialization for Push<GLubyte>
    template <>
    void VertexBufferLayout::Push<GLubyte>(GLuint count)
    {
        m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }

    // Define the rest of the member functions
    std::vector<VertexBufferElement> VertexBufferLayout::GetElements() const
    {
        return m_elements;
    }

    GLuint VertexBufferLayout::GetStride() const
    {
        return m_stride;
    }
}