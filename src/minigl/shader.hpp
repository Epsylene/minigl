#pragma once

#include "geometry.hpp"
#include "texture.hpp"

#include <string>
#include <glad/glad.h>

namespace minigl
{
    enum class ShaderType
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER
    };

    /// Shader abstraction class, in order to easily load and
    /// execute GLSL programs.
    class Shader
    {
        public:

            Shader() = default;

            /// Create a shader from the GLSL file at
            /// `filepath`
            explicit Shader(const std::string& filepath);

            /// Create a shader from a vertex and fragment
            /// source
            Shader(const std::string& vertexSrc,
                   const std::string& fragmentSrc);
            
            /// Destructor: calls `glDeleteProgram()`.
            virtual ~Shader();

            void bind() const;
            void unbind() const;

            void upload(const std::string& name, bool val);
            void upload(const std::string& name, uint32_t val);
            void upload(const std::string& name, int val);
            void upload(const std::string& name, float val);
            void upload(const std::string& name, const Vec2& val);
            void upload(const std::string& name, const Vec3& val);
            void upload(const std::string& name, const Vec4& val);
            void upload(const std::string& name, const Mat3& matrix);
            void upload(const std::string& name, const Mat4& matrix);

            void texture(const std::string& name, Ref<Texture> texture, uint32_t slot = 0);

        private:

            uint32_t shaderID;

            /// Reads the file in `filepath` and returns the
            /// result
            std::string readFile(const std::string& filepath);

            /// Extracts the shaders sources from the file and
            /// puts them in an unordered map, which is
            /// returned
            std::unordered_map<GLenum, std::string> preprocess(const std::string& source);

            /// Compiles the sources from the preprocessing
            /// stage
            void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    };
}