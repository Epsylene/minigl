#include <minigl/minigl.hpp>
using namespace mgl;

int main()
{
    App app { "SSBO", 800, 640 };

    auto compute_shader = ref<Shader>("res/compute.glsl");
    auto image = ref<Texture>(800, 640, TextureFormat::COLOR_RGBA);
    
    auto ifbo = ref<FrameBuffer>();
    ifbo->set_color_attachments_draw({image});
    image->bind_image(0, ImageAccess::WRITE_ONLY);

    struct color_t {
        Color color;
    };
    color_t color = { Color::Green };
    
    auto ssbo = ref<ShaderStorageBuffer<color_t>>(color, 1, DataAccess::MapWrite);

    app.run([&](){
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
    });

    return 0;
}