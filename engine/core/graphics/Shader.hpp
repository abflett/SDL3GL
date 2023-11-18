#pragma once

#include <string>

#include <glad/glad.h>

namespace ige
{
    class Shader
    {
    public:
        Shader(const std::string &filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniforms4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        GLuint GetShader();

    private:
        GLuint m_rendererId;
        std::string m_filePath;

        GLuint CreateShader(const std::string &vertexShader,
                            const std::string &fragmentShader);
        GLuint CompileShader(GLuint type,
                             const std::string &source);
        GLuint GetUniformLocation(const std::string &name);
    };
}