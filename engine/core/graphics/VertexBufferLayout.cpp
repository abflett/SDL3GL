#include "VertexBufferLayout.hpp"

#include "OpenGlUtil.hpp"

namespace ige
{
    VertexBufferLayout::VertexBufferLayout() : m_stride(0) {}

    template <typename T>
    void VertexBufferLayout::Push(GLuint count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::Push");
    }

    template <>
    void VertexBufferLayout::Push<GLfloat>(GLuint count)
    {
        m_elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_stride += OpenGlUtil::GetSizeOfEnumType(GL_FLOAT) * count;
    }

    template <>
    void VertexBufferLayout::Push<GLuint>(GLuint count)
    {
        m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_stride += OpenGlUtil::GetSizeOfEnumType(GL_UNSIGNED_INT) * count;
    }

    template <>
    void VertexBufferLayout::Push<GLubyte>(GLuint count)
    {
        m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_stride += OpenGlUtil::GetSizeOfEnumType(GL_UNSIGNED_BYTE) * count;
    }

    std::vector<VertexBufferElement> VertexBufferLayout::GetElements() const
    {
        return m_elements;
    }

    GLuint VertexBufferLayout::GetStride() const
    {
        return m_stride;
    }
}