#pragma once
#include <glad/glad.h>

namespace minigl
{
    /// Format of the texture as stored on the GPU. This
    /// corresponds to OpenGL's `internalFormat`.
    enum class TextureFormat
    {
        /// 8-bit 3-channel color.
        COLOR_RGB = GL_RGB8,
        /// 32-bit floating point 4-channel color.
        COLOR_RGBA = GL_RGBA32F,
        /// 32-bit floating point depth.
        DEPTH = GL_DEPTH_COMPONENT32F,
    };

    /// Type of access to the image when reading/writing in a
    /// shader.
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

        void bind(uint32_t unit) const;

        /// Bind the texture image for reading/writing in a
        /// shader.
        void bind_image(uint32_t unit, ImageAccess access) const;

        GLuint id;
        int width, height;
        TextureFormat format;
    };
}