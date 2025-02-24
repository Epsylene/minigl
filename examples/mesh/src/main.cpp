#include "minigl/minigl.hpp"

using namespace mgl;

int main() {
    App3D app { "Mesh", 800, 600 };

    Mesh mesh { "res/suzanne.obj" };
    Shader shader { "res/flat.glsl" };

    app.run([&](){
        shader.use();
        shader.upload("u_viewProj", app.camera.viewProj);
        
        mesh.vertexArray->bind();
        RenderCommand::draw_indexed(mesh.vertexArray);
    });

    return 0;
}