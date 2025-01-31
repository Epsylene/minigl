#include "minigl/minigl.hpp"

using namespace minigl;

class TextureApp: public App
{
    public:

        TextureApp(): App(800, 600)
        {
            std::vector<Vertex> vertices {
                {.pos = {-0.5f, -0.75f, 0.0f}, .tex = {0.0f, 0.0f}},
                {.pos = { 0.5f, -0.75f, 0.0f}, .tex = {1.0f, 0.0f}},
                {.pos = { 0.5f,  0.75f, 0.0f}, .tex = {1.0f, 1.0f}},
                {.pos = {-0.5f,  0.75f, 0.0f}, .tex = {0.0f, 1.0f}}
            };

            std::vector<unsigned int> indices {
                0, 1, 2,
                2, 3, 0
            };

            quad = ref<Mesh>(vertices, indices);
            texture = ref<Texture>("res/texture.png");
            shader = ref<Shader>("res/texture.glsl");
        }

        void render() override {
            shader->use();
            texture->bind(0);
            quad->vertexArray->bind();
            RenderCommand::draw_indexed(quad->vertexArray);
        }

    private:
    
        Ref<Shader> shader;
        Ref<Mesh> quad;
        Ref<Texture> texture;
};

int main() {
    TextureApp app {};
    app.run();

    return 0;
}