#include "VertexArray.hpp"
#include "OpenGlUtil.hpp"

namespace ige
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_rendererId);
        glBindVertexArray(m_rendererId);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_rendererId);
    }

    void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
    {
        Bind();
        vb.Bind();

        const auto &elements = layout.GetElements();
        GLuint offset = 0;
        for (GLuint i = 0; i < elements.size(); i++)
        {
            const auto &element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type,
                                  element.normalized, layout.GetStride(), reinterpret_cast<const void *>(static_cast<uintptr_t>(offset)));
            offset += element.count * OpenGlUtil::GetSizeOfEnumType(element.type);
        }
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_rendererId);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}