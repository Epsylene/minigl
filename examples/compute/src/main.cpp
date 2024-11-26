#include <minigl/minigl.hpp>
using namespace minigl;

void print_workgroup_capabilities() {
    info("\nWorkgroup capabilities:");
    info("=======================");

    int workgroup_count[3];
    int workgroup_size[3];
    int workgroup_invocations;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

    info("Workgroup count:");
    info("  x: {}, y: {}, z: {}", workgroup_count[0], workgroup_count[1], workgroup_count[2]);
    
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

    info("Workgroup size:");
    info("  x: {}, y: {}, z: {}", workgroup_size[0], workgroup_size[1], workgroup_size[2]);

    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
    info("Workgroup invocations:");
    info("  {}\n", workgroup_invocations);
}

class Compute: public App {
    public:

        Compute(const int width, const int height):
            App(width, height)
        {
            print_workgroup_capabilities();

            auto vb = ref<VertexBuffer>(
                Buffer<float> {
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
                },
                BufferLayout {
                    {DataType::Float3, "a_pos"},
                    {DataType::Float2, "a_tex"}
                }
            );

            auto ib = ref<IndexBuffer>(
                Buffer<uint32_t> {0, 1, 2, 2, 3, 0}
            );

            quad = ref<VertexArray>(vb, ib);
            
            quad_shader = ref<Shader>("res/quad.glsl");
            compute_shader = ref<Shader>("res/compute.glsl");

            w = 512; h = 512;
            image = ref<Texture>(w, h, TextureFormat::COLOR_RGBA);
            image->bind_image(0, ImageAccess::READ_WRITE);
        }

        void render() override
        {
            // Compute shader
            compute_shader->bind();
            RenderCommand::dispatch_compute(w/16, w/16, 1);
            RenderCommand::memory_barrier();

            // Display
            RenderCommand::clear();
            quad_shader->bind();
            image->bind();
            quad->bind();
            RenderCommand::draw_indexed(quad);
        }

    private:

        Ref<Shader> compute_shader;
        Ref<Shader> quad_shader;
        Ref<VertexArray> quad;
        Ref<Texture> image;
        uint32_t w, h;
};

int main()
{
    Compute app {800, 600};
    app.run();

    return 0;
}