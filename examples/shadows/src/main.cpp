#include "minigl/minigl.hpp"

using namespace mgl;

int main() {
    App3D app { "Shadows", 800, 600 };

    std::vector<Vertex> vertices {
        {.pos = {-10.f, -1.5f, -10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {0.0f, 0.0f}},
        {.pos = { 10.f, -1.5f, -10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {1.0f, 0.0f}},
        {.pos = { 10.f, -1.5f,  10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {1.0f, 1.0f}},
        {.pos = {-10.f, -1.5f,  10.0f}, .normal = {0.f, 1.f, 0.f}, .tex = {0.0f, 1.0f}},
    };

    std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };
    auto ground = ref<Mesh>(vertices, indices);

    auto depth_map = ref<Texture>(1024, 1024, TextureFormat::DEPTH);
    auto shadow_map = ref<FrameBuffer>();
    shadow_map->set_depth_attachment(depth_map);

    auto suzanne = ref<Mesh>("res/suzanne.obj");
    auto scene_shader = ref<Shader>("res/shadows.glsl");
    auto depth_shader = ref<Shader>("res/depth.glsl");

    // Light: use an ortographic camera to simulate a
    // directional light.
    Vec3 light_dir { -2.f, -5.f, -3.f };
    auto light = ref<OrthoCamera>(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
    light->setDirection(light_dir);

    auto render_objects = [&](){
        ground->vertexArray->bind();
        RenderCommand::draw_indexed(ground->vertexArray);

        suzanne->vertexArray->bind();
        RenderCommand::draw_indexed(suzanne->vertexArray);
    };

    app.run([&](){
        // First pass: render the depth map
        depth_shader->use();
        depth_shader->upload("u_lightSpace", light->viewProj);

        RenderCommand::set_viewport(0, 0, 1024, 1024);
        shadow_map->bind();
        RenderCommand::clear(BufferBit::DEPTH);
        render_objects();
        shadow_map->unbind();

        // Second pass: render the actual scene
        RenderCommand::set_viewport(0, 0, 800, 600);
        RenderCommand::clear();

        scene_shader->use();
        scene_shader->upload("u_viewProj", app.camera.viewProj);
        scene_shader->upload("u_lightSpace", light->viewProj);
        scene_shader->upload("u_lightPos", light->getPosition());

        depth_map->bind(0);
        render_objects();
        scene_shader->unbind();
    });

    return 0;
}