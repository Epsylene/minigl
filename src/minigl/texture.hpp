#pragma once

namespace minigl
{
    class Texture
    {
        public:

            /// @brief Create a blank texture of specified
            /// width and height.
            Texture(int width, int height);
            
            /// @brief Create a texture from the image at the
            /// specified path.
            Texture(const std::string& path);

        private:

            unsigned int texture;
            int width, height;
    };
}