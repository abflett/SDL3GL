#include "Engine.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "OpenGlUtil.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include <SDL3_image/SDL_image.h>

namespace ige
{
    Engine::Engine()
        : m_window(SDL_CreateWindow("SDLGL Test", 1200, 600,
                                    SDL_WindowFlags::SDL_WINDOW_OPENGL |
                                        SDL_WindowFlags::SDL_WINDOW_RESIZABLE),
                   &SDL_DestroyWindow),
          m_running(true)
    {
        aspectRatio = 1200.0f / 600.0f;

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef _DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

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

        // SDL_GL_SetSwapInterval(1); // Enable V-Sync

        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_Quit();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _DEBUG
        std::cout << glGetString(GL_VERSION) << "\n";
        if (GLAD_GL_ARB_debug_output)
        {
            glEnable(GL_DEBUG_CALLBACK_FUNCTION_ARB);
            glDebugMessageCallbackARB(OpenGlUtil::DebugOutputCallback, nullptr);
        }
        else
        {
            std::cerr << "GL_ARB_debug_output not supported. Debugging features will be limited." << std::endl;
        }
#endif

        GLfloat positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f};

        GLuint indices[] = {0, 1, 2, 2, 3, 0};

        GLfloat zoom = 0.00f;
        GLfloat zoomPos = 0.05f;

        GLfloat orthSize = 1.0f;
        glm::mat4 projection = glm::ortho(-orthSize, orthSize, -orthSize / aspectRatio, orthSize / aspectRatio, -1.0f, 200.0f);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);
        GLfloat initialOrthoSize = 1.0f;
        GLfloat orthoSize = initialOrthoSize;
        glm::mat4 mvp = glm::ortho(-orthoSize, orthoSize, -orthoSize / aspectRatio, orthoSize / aspectRatio, 0.1f, 200.0f) * view * model;

        VertexArray vao;
        VertexBuffer vbo(positions, sizeof(positions));
        ElementBuffer ebo(indices, sizeof(indices) / sizeof(indices[0]));

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        layout.Push<GLfloat>(2);
        vao.AddBuffer(vbo, layout);

        Shader shader("");
        // shader.SetUniformMatrix4fv("u_MVP", mvp);
        shader.SetUniformMatrix4fv("u_MVP", projection);

        Texture texture("assets/textures/isometric_pixel_0055.png");
        shader.SetUniform1i("u_texture", 0);

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        shader.Unbind();

        Renderer renderer;

        SDL_Event event;
        while (m_running)
        {
            // events
            HandleEvents(event);

            // update
            if (zoom > 10.0f || zoom < 0.0f)
            {
                zoomPos *= -1;
            }
            zoom += zoomPos;
            orthoSize = initialOrthoSize + zoom;
            mvp = glm::ortho(-orthoSize, orthoSize, -orthoSize / aspectRatio, orthoSize / aspectRatio, 0.1f, 200.0f) * view * model;

            // draw
            renderer.Clear();
            shader.Bind();
            shader.SetUniformMatrix4fv("u_MVP", mvp);
            renderer.Draw(vao, ebo, shader);

            SDL_Delay(16);
            SDL_GL_SwapWindow(m_window.get());
        }
    }

    Engine::~Engine()
    {
        SDL_GL_DeleteContext(m_glContext);
        SDL_Quit();
    }

    void Engine::HandleEvents(SDL_Event &event)
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
            if (event.type == SDL_EVENT_WINDOW_RESIZED &&
                event.window.type == SDL_EVENT_WINDOW_RESIZED)
            {
                aspectRatio = static_cast<GLfloat>(event.window.data1) / static_cast<GLfloat>(event.window.data2);
                std::cout << "Aspect Ration Changed: " << aspectRatio << std::endl;
            }
        }
    }
}
