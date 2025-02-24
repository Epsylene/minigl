#include "minigl/minigl.hpp"

int main() {
    // 800x600 window app
    mgl::App app { "Minimal app", 800, 600 };

    app.run([](){
        // Render loop
    });

    return 0;
}