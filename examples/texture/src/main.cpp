#include "minigl/minigl.hpp"

using namespace mgl;

int main() {
    App app { "Texture", 800, 600 };

    std::vector<Vertex> vertices {
        {.pos = {-0.5f, -0.75f, 0.0f}, .tex = {0.0f, 0.0f}},
        {.pos = { 0.5f, -0.75f, 0.0f}, .tex = {1.0f, 0.0f}},
        {.pos = { 0.5f,  0.75f, 0.0f}, .tex = {1.0f, 1.0f}},
        {.pos = {-0.5f,  0.75f, 0.0f}, .tex = {0.0f, 1.0f}}
    };

    std::vector<uint32_t> indices {
        0, 1, 2,
        2, 3, 0
    };

    auto quad = ref<Mesh>(vertices, indices);
    auto texture = ref<Texture>("res/texture.png");
    auto shader = ref<Shader>("res/texture.glsl");

    app.run([&](){
        shader->use();
        texture->bind(0);
        quad->vertexArray->bind();
        RenderCommand::draw_indexed(quad->vertexArray);
    });

    return 0;
}