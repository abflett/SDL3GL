#pragma once

#include <glad/glad.h>

namespace ige
{
    class OpenGlUtil
    {
    public:
        static void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
        template <typename T>
        static GLuint GetEnumType();
        static GLuint GetSizeOfEnumType(GLuint type);

    private:
    };
}