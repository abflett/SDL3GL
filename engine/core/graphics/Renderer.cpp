#include "Renderer.hpp"

#include <glad/glad.h>

namespace ige
{
    void Renderer::Clear() const
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Draw(const VertexArray &vao, const ElementBuffer &ebo, const Shader &shader) const
    {
        shader.Bind();
        vao.Bind();
        ebo.Bind();

        glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr);

        shader.Unbind();
        vao.Unbind();
        ebo.Unbind();
    }
}