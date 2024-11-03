#include "minigl/minigl.hpp"

using namespace minigl;

constexpr uint32_t N = 5;

class InstancesApp: public App3D
{
    public:

        InstancesApp(): App3D(800, 600)
        {
            // Mesh and shader
            mesh = ref<Mesh>("res/suzanne.obj");
            shader = ref<Shader>("res/instances.glsl");
        }

        void render() override {
            shader->bind();
            shader->upload("u_viewProj", camera.viewProj);

            mesh->vertexArray->bind();
            RenderCommand::draw_instanced(mesh->vertexArray, N*N*N);
        }

    private:

        Ref<Shader> shader;
        Ref<Mesh> mesh;
};

int main() {
    InstancesApp app {};
    app.run();

    return 0;
}