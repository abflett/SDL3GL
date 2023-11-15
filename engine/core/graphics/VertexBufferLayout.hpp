#pragma once

#include <vector>
#include <glad/glad.h>

namespace ige
{
    struct VertexBufferElement
    {
        GLuint type;
        GLuint count;
        GLboolean normalized;

        static GLuint GetSizeOfType(GLuint type)
        {
            switch (type)
            {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
            default:
                return 0;
            }
        }
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout()
            : m_stride(0)
        {
        }

        ~VertexBufferLayout() {}

        template <typename T>
        void Push(GLuint count)
        {
            static_assert(false, "Unsupported type for VertexBufferLayout::Push");
        }

        template <>
        void Push<GLfloat>(GLuint count)
        {
            m_elements.push_back({GL_FLOAT, count, GL_FALSE});
            m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
        }

        template <>
        void Push<GLuint>(GLuint count)
        {
            m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
        }

        template <>
        void Push<GLubyte>(GLuint count)
        {
            m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
            m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
        }

        GLuint GetStride() const
        {
            return m_stride;
        }

        std::vector<VertexBufferElement> GetElements() const
        {
            return m_elements;
        }

    private:
        std::vector<VertexBufferElement> m_elements;
        GLuint m_stride;
    };
}