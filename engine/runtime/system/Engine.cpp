#include "Engine.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string ReadFile(const std::string &filepath)
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
        : m_window(SDL_CreateWindow("SDLGL Test", 800, 600,
                                    SDL_WindowFlags::SDL_WINDOW_OPENGL |
                                        SDL_WindowFlags::SDL_WINDOW_RESIZABLE),
                   &SDL_DestroyWindow)
    {
        std::cout << "Starting Engine..."
                  << "\n";

        VertexBuffer vb;

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

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

        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_Quit();
        }

#ifdef _DEBUG
        std::cout << glGetString(GL_VERSION) << "\n";
#endif

        GLfloat positions[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};

        GLuint indices[] = {0, 1, 2, 2, 3, 0};

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        glEnableVertexAttribArray(0);

        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        glBindVertexArray(0);

        std::string vertexShaderSource = ReadFile("assets/shaders/Basic.vert");
        std::string fragmentShaderSource = ReadFile("assets/shaders/Basic.frag");

        GLuint shader = CreateShader(vertexShaderSource, fragmentShaderSource);
        glUseProgram(shader);

        int uniformLocation = glGetUniformLocation(shader, "u_color");
        glUniform4f(uniformLocation, 0.2f, 0.3f, 0.8f, 1.0f);

        GLfloat red = 0.00f;
        GLfloat r_increment = 0.001f;

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
            glBindVertexArray(vao);

            if (red > 1.0f || red < 0.0f)
            {
                r_increment *= -1;
            }
            red += r_increment;
            glUniform4f(uniformLocation, red, 0.3f, 0.8f, 1.0f);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
                           GL_UNSIGNED_INT, nullptr);

            glBindVertexArray(0);
            glUseProgram(0);
            SDL_GL_SwapWindow(m_window.get());
        }

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shader);
    }
    Engine::~Engine()
    {
        SDL_GL_DeleteContext(m_glContext);
        SDL_Quit();
    }
} // namespace ige