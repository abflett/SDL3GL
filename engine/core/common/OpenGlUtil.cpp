#include "OpenGlUtil.hpp"

#include <type_traits>
#include <iostream>

namespace ige
{
    void APIENTRY OpenGlUtil::DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
        bool g_suppressInfoMessages = true;
        // Convert GLenum values to human-readable strings
        std::string sourceStr, typeStr, severityStr;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API_ARB:
            sourceStr = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            sourceStr = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            sourceStr = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            sourceStr = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            sourceStr = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER_ARB:
            sourceStr = "Other";
            break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR_ARB:
            typeStr = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            typeStr = "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            typeStr = "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            typeStr = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            typeStr = "Performance";
            break;
        case GL_DEBUG_TYPE_OTHER_ARB:
            typeStr = "Other";
            break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            severityStr = "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            severityStr = "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW_ARB:
            severityStr = "Low";
            break;
        default:
            severityStr = "Unknown";
            break;
        }

        // Check if the message should be suppressed based on severity and the global flag
        if (g_suppressInfoMessages && severityStr == "Unknown")
        {
            // Suppress informational messages
            return;
        }

        // Print the debug output details
        std::cerr << "OpenGL Debug Output:" << std::endl;
        std::cerr << "  Source: " << sourceStr << std::endl;
        std::cerr << "  Type: " << typeStr << std::endl;
        std::cerr << "  ID: " << id << std::endl;
        std::cerr << "  Severity: " << severityStr << std::endl;
        std::cerr << "  Message: " << message << std::endl;
    }

    template <typename T>
    GLuint OpenGlUtil::GetEnumType()
    {
        static_assert(std::is_same<T, GLfloat>() ||
                          std::is_same<T, GLuint>() ||
                          std::is_same<T, GLubyte>(),
                      "Unsupported type for GetEnumType");

        if constexpr (std::is_same<T, GLfloat>())
        {
            return GL_FLOAT;
        }
        else if constexpr (std::is_same<T, GLuint>())
        {
            return GL_UNSIGNED_INT;
        }
        else if constexpr (std::is_same<T, GLubyte>())
        {
            return GL_UNSIGNED_BYTE;
        }
        else
        {
            // This should never be reached due to the static_assert
            return 0;
        }
    }

    GLuint OpenGlUtil::GetSizeOfEnumType(GLuint type)
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