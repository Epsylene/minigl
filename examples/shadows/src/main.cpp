#include "minigl/minigl.hpp"

using namespace minigl;

class ShadowsApp: public App3D
{
    public:

        ShadowsApp(): App3D(800, 600)
        {
            std::vector<Vertex> vertices {
                {.pos = {-10.f, -1.5f, -10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {0.0f, 0.0f}},
                {.pos = { 10.f, -1.5f, -10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {1.0f, 0.0f}},
                {.pos = { 10.f, -1.5f,  10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {1.0f, 1.0f}},
                {.pos = {-10.f, -1.5f,  10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {0.0f, 1.0f}},
            };

            std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };
            ground = ref<Mesh>(vertices, indices);

            depth_map = ref<Texture>(1024, 1024, TextureFormat::DEPTH);
            shadow_map = ref<FrameBuffer>();
            shadow_map->attach_depth_texture(depth_map);

            suzanne = ref<Mesh>("res/suzanne.obj");
            scene_shader = ref<Shader>("res/shadows.glsl");
            depth_shader = ref<Shader>("res/depth.glsl");

            // Light: use an ortographic camera to simulate a
            // directional light.
            Vec3 light_dir { -2.f, -5.f, -3.f };
            light = ref<OrthoCamera>(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
            light->setDirection(light_dir);
        }

        void render() override {
            // First pass: render the depth map
            depth_shader->bind();
            depth_shader->upload("u_lightSpace", light->viewProj);

            RenderCommand::set_viewport(0, 0, 1024, 1024);
            shadow_map->bind();
            RenderCommand::clear(GL_DEPTH_BUFFER_BIT);
            render_objects();
            shadow_map->unbind();

            // Second pass: render the actual scene
            RenderCommand::set_viewport(0, 0, 800, 600);
            RenderCommand::clear();

            scene_shader->bind();
            scene_shader->upload("u_viewProj", camera.viewProj);
            scene_shader->upload("u_lightSpace", light->viewProj);
            scene_shader->upload("u_lightPos", light->getPosition());

            depth_map->bind();
            render_objects();
            scene_shader->unbind();
        }

        void render_objects() {
            ground->vertexArray->bind();
            RenderCommand::draw_indexed(ground->vertexArray);

            suzanne->vertexArray->bind();
            RenderCommand::draw_indexed(suzanne->vertexArray);
        }

    private:
    
        Ref<Shader> scene_shader;
        Ref<Shader> depth_shader;

        Ref<Texture> depth_map;
        Ref<FrameBuffer> shadow_map;
        
        Ref<Mesh> suzanne;
        Ref<Mesh> ground;

        Ref<OrthoCamera> light;
};

int main() {
    ShadowsApp app {};
    app.run();

    return 0;
}