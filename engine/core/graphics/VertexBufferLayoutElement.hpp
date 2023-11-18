#pragma once

#include <glad/glad.h>

namespace ige
{
    struct VertexBufferElement
    {
        GLuint type;
        GLuint count;
        GLboolean normalized;
    };
}