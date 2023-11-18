#include "VertexBufferElement.hpp"

namespace ige
{
    // Define the static member function of VertexBufferElement
    GLuint VertexBufferElement::GetSizeOfType(GLuint type)
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
}