#pragma once

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "Shader.hpp"

namespace ige
{
    class Renderer
    {
    public:
        void Clear() const;
        void Draw(const VertexArray &vao, const ElementBuffer &ebo, const Shader &shader) const;

    private:
    };
}