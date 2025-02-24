#include "minigl/minigl.hpp"

using namespace mgl;

int main() {
    App app { "Triangle", 800, 600 };

    // Vertices
    std::vector<Vertex> vertices = {
        // Position, Normal, Texcoord, Color
        {.pos = {-0.5f, -0.5f, 0.0f}, .color = {1.0, 0.0, 0.0}},
        {.pos = { 0.5f, -0.5f, 0.0f}, .color = {0.0, 1.0, 0.0}},
        {.pos = { 0.0f,  0.5f, 0.0f}, .color = {0.0, 0.0, 1.0}}
    };
    
    std::vector<uint32_t> indices { 0, 1, 2 };

    // Vertex buffer and index buffer
    auto ib = ref<IndexBuffer>(indices);          
    auto vb = ref<VertexBuffer>(vertices);

    // Vertex array and shader
    auto tri_va = ref<VertexArray>(vb, ib);
    auto shader = ref<Shader>("res/triangle.glsl");

    app.run([&](){
        shader->use();
        tri_va->bind();
        RenderCommand::draw_indexed(tri_va);
    });

    return 0;
}