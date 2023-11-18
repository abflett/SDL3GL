#include "Shader.hpp"

#include "FileUtil.hpp"

#include <vector>
#include <iostream>

namespace ige
{
    Shader::Shader(const std::string &filepath) : m_rendererId(0), m_filePath(filepath)
    {
        std::string vertexShaderSource = FileUtil::ReadFileToString("assets/shaders/Basic.vert");
        std::string fragmentShaderSource = FileUtil::ReadFileToString("assets/shaders/Basic.frag");

        m_rendererId = CreateShader(vertexShaderSource, fragmentShaderSource);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_rendererId);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_rendererId);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniforms4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    GLuint Shader::GetShader()
    {
        return m_rendererId;
    }

    GLuint Shader::GetUniformLocation(const std::string &name)
    {
        return glGetUniformLocation(m_rendererId, name.c_str());
    }

    GLuint Shader::CreateShader(const std::string &vertexShader,
                                const std::string &fragmentShader)
    {
        GLuint program = glCreateProgram();
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        // Check the link status
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            GLint logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength + 1);
            glGetProgramInfoLog(program, logLength, nullptr, log.data());

            // Print the linking log
            std::cerr << "Failed to link program: " << log.data() << std::endl;

            glDeleteProgram(program);
            glDeleteShader(vs);
            glDeleteShader(fs);

            return 0;
        }

        glValidateProgram(program);

        glDetachShader(program, vs);
        glDetachShader(program, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    GLuint Shader::CompileShader(GLuint type,
                                 const std::string &source)
    {
        GLuint id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        GLint compileStatus;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE)
        {
            GLint logLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength + 1);
            glGetShaderInfoLog(id, logLength, nullptr, log.data());

            // Print the compilation log
            std::cerr << "Failed to compile "
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                      << " shader: " << log.data() << std::endl;

            glDeleteShader(id);
            return 0;
        }

        return id;
    }

}