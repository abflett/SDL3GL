#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

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
        void SetUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix);

    private:
        GLuint m_rendererId;
        std::string m_filePath;
        std::unordered_map<std::string, GLuint> m_uniformLocationCache;

        GLuint CreateShader(const std::string &vertexShader,
                            const std::string &fragmentShader);
        GLuint CompileShader(GLuint type,
                             const std::string &source);
        GLuint GetUniformLocation(const std::string &name);
    };
}