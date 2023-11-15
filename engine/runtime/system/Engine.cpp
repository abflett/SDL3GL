#include "Engine.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
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
    }

    // Print the debug output details
    std::cerr << "OpenGL Debug Output:" << std::endl;
    std::cerr << "  Source: " << sourceStr << std::endl;
    std::cerr << "  Type: " << typeStr << std::endl;
    std::cerr << "  ID: " << id << std::endl;
    std::cerr << "  Severity: " << severityStr << std::endl;
    std::cerr << "  Message: " << message << std::endl;
}

static std::string ReadFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open file " << filepath << std::endl;
        return "";
    }

    std::stringstream ss;
    std::string line;
    while (getline(file, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

static GLuint CompileShader(GLuint type, const std::string &source)
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

static GLuint CreateShader(const std::string &vertexShader,
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

namespace ige
{
    Engine::Engine()
        : m_window(SDL_CreateWindow("SDLGL Test", 1200, 600,
                                    SDL_WindowFlags::SDL_WINDOW_OPENGL |
                                        SDL_WindowFlags::SDL_WINDOW_RESIZABLE),
                   &SDL_DestroyWindow)
    {
        std::cout << "Starting Engine..."
                  << "\n";
        float aspectRatio = 1200.0f / 600.0f;

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError()
                      << std::endl;
        }

        if (!m_window)
        {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }

        m_glContext = SDL_GL_CreateContext(m_window.get());
        if (!m_glContext)
        {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
        }

        SDL_GL_SetSwapInterval(1); // Enable V-Sync

        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_Quit();
        }

#ifdef _DEBUG
        std::cout << glGetString(GL_VERSION) << "\n";

        if (GLAD_GL_ARB_debug_output)
        {
            // Enable the debug output
            glEnable(GL_DEBUG_CALLBACK_FUNCTION_ARB);
            glDebugMessageCallbackARB(glDebugOutput, nullptr);
        }
        else
        {
            std::cerr << "GL_ARB_debug_output not supported. Debugging features will be limited." << std::endl;
        }
#endif

        GLfloat positions[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f};

        GLuint indices[] = {0, 1, 2, 2, 3, 0};

        VertexArray vao;
        VertexBuffer vbo(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        vao.AddBuffer(vbo, layout);

        ElementBuffer ebo(indices, sizeof(indices) / sizeof(indices[0]));

        vbo.Unbind();

        std::string vertexShaderSource = ReadFile("assets/shaders/Basic.vert");
        std::string fragmentShaderSource = ReadFile("assets/shaders/Basic.frag");

        GLuint shader = CreateShader(vertexShaderSource, fragmentShaderSource);

        int mvpLocation = glGetUniformLocation(shader, "u_MVP");
        int uniformLocation = glGetUniformLocation(shader, "u_color");

        // Outside of the rendering loop
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Set up the model matrix (you can adjust this based on your needs)
        glm::mat4 model = glm::mat4(1.0f);

        float initialOrthoSize = 2.0f;
        float orthoSize = initialOrthoSize;

        GLfloat red = 0.00f;
        GLfloat r_increment = 0.01f;

        SDL_Event event;
        bool m_running = true;
        while (m_running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    m_running = false;
                }
                if (event.type == SDL_EVENT_KEY_DOWN)
                {
                    auto keysympressed = event.key.keysym.sym;
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        m_running = false;
                    }
                }
            }
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);

            // Update only the dynamic parts (e.g., the red factor) within the loop
            glUniform4f(uniformLocation, red, 0.3f, 0.8f, 1.0f);

            vao.Bind();
            ebo.Bind();

            // Calculate the Model-View-Projection (MVP) matrix
            glm::mat4 mvp = glm::ortho(-orthoSize, orthoSize, -orthoSize / aspectRatio, orthoSize / aspectRatio, 0.1f, 100.0f) * view * model;

            // Pass the MVP matrix to the shader
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

            if (red > 1.0f || red < 0.0f)
            {
                r_increment *= -1;
            }
            red += r_increment;

            // Dynamic zooming
            orthoSize = initialOrthoSize + red;

            glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr);

            ebo.Unbind();
            glBindVertexArray(0);
            glUseProgram(0);
            SDL_GL_SwapWindow(m_window.get());
            SDL_Delay(16);
        }

        glDeleteProgram(shader);
    }

    Engine::~Engine()
    {
        SDL_GL_DeleteContext(m_glContext);
        SDL_Quit();
    }
}