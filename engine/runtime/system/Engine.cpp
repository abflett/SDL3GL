#include "Engine.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "OpenGlUtil.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "InputManager.hpp"
#include "FpsCounter.hpp"

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
        : m_window(1200, 600, "SDL3GL Demo"),
          m_running(true)
    {
        m_aspectRatio = 1200.0f / 600.0f;

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
    }

    Engine::~Engine()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_glContext);
        SDL_Quit();
    }

    void Engine::Run()
    {

        const char *glsl_version = "#version 330 core";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplSDL3_InitForOpenGL(m_window.GetSDLWindow(), m_glContext);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        GLfloat positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f};

        GLuint indices[] = {0, 1, 2, 2, 3, 0};

        // Todo: Normalize the projection so pixels are true for setting max and min zoom levels.
        // Set the desired pixel density (64x64 area corresponds to 1 unit)
        float pixelsPerUnit = 64.0f;
        // Calculate the orthographic size
        m_orthoSize = (float)600 / (4 * pixelsPerUnit);
        // Adjust orthoSize based on the actual aspect ratio
        if (m_aspectRatio > 1.0f)
        {
            m_orthoSize *= m_aspectRatio;
        }
        // m_orthoSize = 4.0f;
        m_mvp = glm::ortho(-m_orthoSize * m_aspectRatio, m_orthoSize * m_aspectRatio, -m_orthoSize, m_orthoSize, -1.0f, 200.0f);

        VertexArray vao;
        VertexBuffer vbo(positions, sizeof(positions));
        ElementBuffer ebo(indices, sizeof(indices) / sizeof(indices[0]));

        VertexBufferLayout layout;
        layout.Push<GLfloat>(2);
        layout.Push<GLfloat>(2);
        vao.AddBuffer(vbo, layout);

        Shader shader("");
        shader.SetUniformMatrix4fv("u_MVP", m_mvp);

        Texture texture("assets/textures/green_outline_block_001.png");
        shader.SetUniform1i("u_texture", 0);

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        shader.Unbind();

        Renderer renderer;
        FpsCounter fpsCounter;
        InputManager inputManager(m_running);

        while (m_running)
        {
            // events
            inputManager.HandleEvents();

            // update
            fpsCounter.Update();

            // draw
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            // ImGui::ShowDemoWindow();
            ImGui::Begin("Debug Info");
            ImGui::Text("FPS: %.1f", fpsCounter.GetFPS());
            ImGui::Text("Average FPS: %.1f", fpsCounter.GetAverageFPS());
            ImGui::Text("Highest FPS: %.1f", fpsCounter.GetHighestFPS());
            ImGui::Text("Lowest FPS: %.1f", fpsCounter.GetLowestFPS());
            ImGui::End();
            ImGui::Render();

            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            renderer.Clear();
            shader.Bind();
            shader.SetUniformMatrix4fv("u_MVP", m_mvp);
            renderer.Draw(vao, ebo, shader);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(m_window.GetSDLWindow());
        }
    }
}
