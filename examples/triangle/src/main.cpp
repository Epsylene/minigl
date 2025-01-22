#include "minigl/minigl.hpp"

using namespace minigl;

class Triangle: public App {
    public:
    
        Triangle(): App(800, 600) 
        {
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
            tri_va = ref<VertexArray>(vb, ib);
            shader = ref<Shader>("res/triangle.glsl");
        }

        void render() override {
            // Draw the triangle
            tri_va->bind();
            RenderCommand::draw_indexed(tri_va);
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