#include "minigl/minigl.hpp"

using namespace mgl;

constexpr uint32_t N = 5;

int main() {
    App3D app { "Instancing", 800, 600 };

    auto mesh = ref<Mesh>("res/suzanne.obj");
    auto shader = ref<Shader>("res/instances.glsl");

    app.run([&](){
        shader->use();
        mesh->vertexArray->bind();
        RenderCommand::draw_instanced(mesh->vertexArray, N*N*N);
    });

    return 0;
}