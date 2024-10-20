#pragma once
#include <glad/glad.h>

namespace minigl
{
    enum class TextureFormat
    {
        COLOR = GL_RGB8,
        DEPTH = GL_DEPTH_COMPONENT32F,
    };

    struct Texture
    {
        /// Create a blank texture of specified width and
        /// height.
        Texture(int width, int height, TextureFormat type);
        
        /// Create a texture from the image at the
        /// specified path.
        Texture(const std::string& path);

        void bind(uint32_t unit) const;

        GLuint id;
        int width, height;
    };
}