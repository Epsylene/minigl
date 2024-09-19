#include "minigl/minigl.hpp"

using namespace minigl;

class MeshApp: public App3D{
    public:
        MeshApp(): App3D(800, 600) 
        {
            // Set initial camera position
            camera.setPosition({0.f, 0.f, 3.f});

            // Mesh and shader
            mesh = ref<Mesh>("res/suzanne.obj");
            shader = ref<Shader>("res/flat.glsl");
        }

        void render() override {
            shader->bind();
            shader->uniformMat4("u_viewProj", camera.viewProj);

            mesh->vertexArray->bind();
            RenderCommand::draw_indexed(mesh->vertexArray);
        }

    private:
        Ref<Shader> shader;
        Ref<Mesh> mesh;
};

int main() {
    MeshApp app {};
    app.run();

    return 0;
}