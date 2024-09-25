#include "texture.hpp"

#include <glad/glad.h>

namespace minigl
{
    Texture::Texture(int width, int height): width(width), height(height)
    {
        // Create a 2D texture handle with the retrieved ID.
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        // Allocate storage for width x height pixels in RGB8
        // format and 1 mip level.
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);

        // Set the min and mag filters to linear (linear
        // interpolation when the texels are smaller or larger
        // than the screen pixels).
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, texture);
    }

    Texture::Texture(const std::string& path)
    {
        // Create a texture from the image at the specified path.
    }
}