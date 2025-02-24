#include "minigl/minigl.hpp"

class MinApp: public mgl::App {
    public:
        // 800x600 window
        MinApp(): mgl::App(800, 600) {}

        void render() override {
            // Draw stuff
        }
};

int main() {
    MinApp app {};
    app.run();

    return 0;
}