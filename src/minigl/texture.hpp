#pragma once
#include <glad/glad.h>

namespace minigl
{
    enum class TextureFormat
    {
        COLOR_RGB = GL_RGB8,
        COLOR_RGBA = GL_RGBA32F,
        DEPTH = GL_DEPTH_COMPONENT32F,
    };

    enum class ImageAccess
    {
        READ_ONLY = GL_READ_ONLY,
        WRITE_ONLY = GL_WRITE_ONLY,
        READ_WRITE = GL_READ_WRITE,
    };

    struct Texture
    {
        /// Create a blank texture of specified width and
        /// height.
        Texture(int width, int height, TextureFormat type);
        
        /// Create a texture from the image at the
        /// specified path.
        Texture(const std::string& path);

        void bind(uint32_t unit = 0) const;
        void bind_image(uint32_t unit, ImageAccess access) const;

        GLuint id;
        int width, height;
        TextureFormat format;
    };
}