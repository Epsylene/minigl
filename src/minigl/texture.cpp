#include "texture.hpp"

#include "core.hpp"
#include <glad/glad.h>
#include <stb_image.h>

namespace minigl
{
    Texture::Texture(int width, int height, TextureFormat format):
        width(width), height(height), format(format)
    {
        // Create a 2D texture handle with the retrieved ID.
        glCreateTextures(GL_TEXTURE_2D, 1, &id);

        // Allocate storage for width x height pixels in the
        // given format and with 1 mip level.
        glTextureStorage2D(id, 1, (GLenum)format, width, height);

        // Set the min and mag filters to linear (perform
        // linear interpolation when the texels are smaller or
        // larger than the screen pixels).
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set the wrap mode to repeat (repeat the texture when
        // the texture coordinates are outside the [0, 1]
        // range).
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        
        // Tell how many mipmaps levels there are, the internal
        // format of the texture (that is, how it shall be
        // stored in GPU), its width and its height.
        glTextureStorage2D(id, 1, internalFormat, width, height);

        // Magnification and minification filter, that is,
        // which algorithms are used to make our texture bigger
        // or smaller.
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Specify the texture sub-image that will be used: the
        // pixel data, its format (memory layout), and a type.
        // The pixel data will be converted from 'dataformat'
        // to 'internalformat' in order to be used by OpenGL.
        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);

        trace("Created texture from image at path '{}'", path);
    }

    void Texture::bind(uint32_t unit) const
    {
        glBindTextureUnit(unit, id);
    }

    void Texture::bind_image(uint32_t unit, ImageAccess access) const
    {
        // The format could actually be different from the
        // texture format, as long as it is compatible (see
        // https://docs.gl/gl4/glBindImageTexture#description).
        glBindImageTexture(unit, id, 0, GL_FALSE, 0, (GLenum)access, (GLenum)format);
    }
}