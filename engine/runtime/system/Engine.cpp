#include "Engine.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "OpenGlUtil.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <deque>
#include <chrono>
#include <iostream>
#include <string>

#include <SDL3_image/SDL_image.h>

namespace ige
{
    Engine::Engine()
        : m_window(1200, 600, "SDLGL Test"),
          m_running(true)
    {
        aspectRatio = 1200.0f / 600.0f;
        const char *glsl_version = "#version 330 core";

        m_glContext = SDL_GL_CreateContext(m_window.GetSDLWindow());
        if (!m_glContext)
        {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                      << std::endl;
            SDL_Quit();
        }

        SDL_GL_MakeCurrent(m_window.GetSDLWindow(), m_glContext);
        SDL_GL_SetSwapInterval(0);
        SDL_ShowWindow(m_window.GetSDLWindow());

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_Quit();
        }

        ImGui_ImplSDL3_InitForOpenGL(m_window.GetSDLWindow(), m_glContext);
        ImGui_ImplOpenGL3_Init(glsl_version);

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

        m_orthoSize = 10.0f;
        m_mvp = glm::ortho(-m_orthoSize * aspectRatio, m_orthoSize * aspectRatio, -m_orthoSize, m_orthoSize, -1.0f, 200.0f);

        VertexArray vao;
        VertexBuffer vbo(positions, sizeof(positions));
        ElementBuffer ebo(indices, sizeof(indices) / sizeof(indices[0]));

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        layout.Push<GLfloat>(2);
        vao.AddBuffer(vbo, layout);

        Shader shader("");
        // shader.SetUniformMatrix4fv("u_MVP", mvp);
        shader.SetUniformMatrix4fv("u_MVP", m_mvp);

        Texture texture("assets/textures/green_outline_block_001.png");
        shader.SetUniform1i("u_texture", 0);

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        shader.Unbind();

        Renderer renderer;

        auto startTime = std::chrono::high_resolution_clock::now();
        std::deque<float> fpsHistory; // Store recent FPS values for averaging

        float highestFPS = 0.0f;
        float lowestFPS = std::numeric_limits<float>::max(); // Initialize with a large value

        SDL_Event event;
        while (m_running)
        {
            // events
            HandleEvents(event);

            // Calculate FPS information
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            float fps = 1.0f / deltaTime;
            fpsHistory.push_front(fps);
            if (SDL_GetTicks() > 1000)
            {
                highestFPS = std::max(highestFPS, fps);
                lowestFPS = std::min(lowestFPS, fps);
            }
            const size_t maxHistorySize = 5000;
            while (fpsHistory.size() > maxHistorySize)
            {
                fpsHistory.pop_back();
            }
            float averageFPS = 0.0f;
            for (float value : fpsHistory)
                averageFPS += value;
            averageFPS /= static_cast<float>(fpsHistory.size());
            startTime = currentTime;

            // Init ImgGui frames?
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow();
            ImGui::Begin("Debug Info");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Average FPS: %.1f", averageFPS);
            ImGui::Text("Highest FPS: %.1f", highestFPS);
            ImGui::Text("Lowest FPS: %.1f", lowestFPS);
            ImGui::End();
            ImGui::Render();

            // update

            // draw

            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            renderer.Clear();
            shader.Bind();
            shader.SetUniformMatrix4fv("u_MVP", m_mvp);
            renderer.Draw(vao, ebo, shader);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(m_window.GetSDLWindow());
        }
    }

    Engine::~Engine()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_glContext);
        SDL_Quit();
    }

    void Engine::Initialize()
    {
    }

    void Engine::Run()
    {
    }

    void Engine::HandleEvents(SDL_Event &event)
    {

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
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

                GLfloat orthoSizeX = m_orthoSize * aspectRatio;
                m_mvp = glm::ortho(-orthoSizeX, orthoSizeX, -m_orthoSize, m_orthoSize, -1.0f, 200.0f);
            }
        }
    }
}
