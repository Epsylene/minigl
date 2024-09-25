#include "texture.hpp"

#include "core.hpp"
#include <glad/glad.h>
#include <stb_image.h>

namespace minigl
{
    Texture::Texture(int width, int height): width(width), height(height)
    {
        // Create a 2D texture handle with the retrieved ID.
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        // Allocate storage for width x height pixels in RGB8
        // format and 1 mip level.
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);

        // Set the min and mag filters to linear (perform
        // linear interpolation when the texels are smaller or
        // larger than the screen pixels).
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Bind the texture to OpenGL.
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    Texture::Texture(const std::string& path)
    {
        // Load the texture with stbi. The coordinates are
        // flipped because OpenGL expects the origin to be at
        // the bottom left corner.
        int channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        // - Data format: format of the pixel data.
        // - Internal format: format used by OpenGL to store
        //   the texture.
        GLenum dataFormat = 0, internalFormat = 0;
        if(channels == 3)
        {
            dataFormat = GL_RGB;
            internalFormat = GL_RGB8;
        }
        else if(channels == 4)
        {
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA8;
        }
        MGL_ASSERT(internalFormat & dataFormat, "Texture at path '{}': format not supported or texture not found.", path);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Generate and bind the texture to OpenGL.
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        
        // Tell how many mipmaps levels there are, the internal
        // format of the texture (that is, how it shall be
        // stored in GPU), its width and its height.
        glTextureStorage2D(texture, 1, internalFormat, width, height);

        // Magnification and minification filter, that is,
        // which algorithms are used to make our texture bigger
        // or smaller.
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Specify the texture sub-image that will be used: the
        // pixel data, its format (memory layout), and a type.
        // The pixel data will be converted from 'dataformat'
        // to 'internalformat' in order to be used by OpenGL.
        glTextureSubImage2D(texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    void Texture::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}