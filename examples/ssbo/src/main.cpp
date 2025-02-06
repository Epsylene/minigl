#include <minigl/minigl.hpp>
using namespace minigl;

class Compute: public App {
    public:

        Compute(const int width, const int height):
            App(width, height)
        {
            compute_shader = ref<Shader>("res/compute.glsl");

            image = ref<Texture>(width, height, TextureFormat::COLOR_RGBA);
            
            ifbo = ref<FrameBuffer>();
            ifbo->set_color_attachments_draw({image});
            image->bind_image(0, ImageAccess::WRITE_ONLY);

            color_t color = { Color::Green };
            ssbo = ref<ShaderStorageBuffer<color_t>>(color, 1, DataAccess::MapWrite);
        }

        void render() override
        {
            compute_shader->use();

            // Change color over time
            Color color = {
                0.5f * glm::sin(App::time()) + 0.5f,
                0.5f * glm::cos(App::time()) + 0.5f,
                0.0f,
                1.0f
            };
            ssbo->write(0, { color });

            auto [w, h] = ifbo->size();
            RenderCommand::dispatch_compute(w/16, h/16, 1);
            RenderCommand::memory_barrier();

            ifbo->blit_to_default(BufferBit::COLOR, w, h);
        }

    private:

        Ref<Shader> compute_shader;

        struct color_t {
            Color color;
        };
        Ref<ShaderStorageBuffer<color_t>> ssbo;

        Ref<FrameBuffer> ifbo;
        Ref<Texture> image;
};

int main()
{
    Compute app {800, 640};
    app.run();

    return 0;
}