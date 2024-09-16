#include "glose/app/app.hpp"

class MinApp: public glose::App {
    public:
        // 800x600 window
        MinApp(): glose::App(800, 600) {}

        void render() override {
            // Draw stuff
        }
};

int main() {
    MinApp app {};
    app.run();

    return 0;
}