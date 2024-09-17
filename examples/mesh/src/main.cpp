#include "glose/glose.hpp"

using namespace glose;

class MeshApp: public App3D {
    public:
        MeshApp(): App3D(800, 600) 
        {
            std::vector<Vertex> vertices {
                {{-0.5f, -0.5f, -0.5f}, {}, {}, {1.0, 0.0, 0.0}},
                {{ 0.5f, -0.5f, -0.5f}, {}, {}, {0.0, 1.0, 0.0}},
                {{ 0.5f, -0.5f,  0.5f}, {}, {}, {0.0, 0.0, 1.0}},
                {{-0.5f, -0.5f,  0.5f}, {}, {}, {1.0, 1.0, 1.0}},
                {{-0.5f,  0.5f, -0.5f}, {}, {}, {1.0, 0.0, 0.0}},
                {{ 0.5f,  0.5f, -0.5f}, {}, {}, {0.0, 1.0, 1.0}},
                {{ 0.5f,  0.5f,  0.5f}, {}, {}, {0.0, 0.0, 1.0}},
                {{-0.5f,  0.5f,  0.5f}, {}, {}, {1.0, 1.0, 1.0}},
            };

            std::vector<uint32_t> indices = {
                0, 1, 2, // Side 0
                2, 0, 3,
                4, 0, 1, // Side 1
                1, 5, 4,
                5, 1, 2, // Side 2
                2, 6, 5,
                3, 2, 3, // Side 3
                3, 7, 6,
                4, 5, 6, // Side 4
                6, 7, 4,
                0, 4, 7, // Side 5
                7, 3, 0
            };

            // Mesh
            mesh = ref<Mesh>(vertices, indices);
            
            // Shader
            shader = ref<Shader>("res/mesh.glsl");
            shader->bind();
            shader->uniformFloat3("u_color", Color::Blue);
        }

        void render() override {
            shader->bind();
            shader->uniformMat4("u_viewProj", camera.viewProjMatrix);

            mesh->vertexArray->bind();
            RenderCommand::drawIndexed(mesh->vertexArray);
        }

    private:
        Ref<Shader> shader;
        Ref<Mesh> mesh;
        Ref<VertexArray> tri_va;
};

int main() {
    MeshApp app {};
    app.run();

    return 0;
}