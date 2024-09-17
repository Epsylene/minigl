#include "glose/glose.hpp"

using namespace glose;

class Triangle: public App {
    public:
        Triangle(): App(800, 600) 
        {
            // Vertices and indices
            float tri_vtx[] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.0f,  0.5f, 0.0f
            };
            auto tri_idx = std::vector<uint32_t> { 0, 1, 2 };
 
            // Vertex buffer and index buffer
            auto ib = ref<IndexBuffer>(tri_idx);            
            auto vb = ref<VertexBuffer>(tri_vtx, sizeof(tri_vtx));
            vb->setLayout({{DataType::Float3, "a_position"}});

            // Vertex array and shader
            tri_va = ref<VertexArray>(vb, ib);
            shader = ref<Shader>("res/triangle.glsl");

            // Shader uniforms
            shader->bind();
            shader->uniformFloat3("u_color", Color::Red);
        }

        void render() override {
            tri_va->bind();
            RenderCommand::drawIndexed(tri_va, Primitives::TRIANGLES);
        }

    private:
        Ref<VertexArray> tri_va;
        Ref<Shader> shader;
};

int main() {
    Triangle app {};
    app.run();

    return 0;
}