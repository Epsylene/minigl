#pragma once

#include "geometry.hpp"
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

    /// @brief Shader abstraction class
    class Shader
    {
        public:

            /// @brief Create a shader from the GLSL file at
            /// `filepath`
            explicit Shader(const std::string& filepath);

            /// @brief Create a shader from a vertex and
            /// fragment source
            /// 
            /// @param vertexSrc Vertex shader source code
            /// @param fragmentSrc Fragment shader source code
            Shader(const std::string& vertexSrc,
                   const std::string& fragmentSrc);
            
            /// @brief Shader destructor
            ///
            /// @details Calls glDeleteProgram() on 'textureID`.
            virtual ~Shader();

            void bind() const;
            void unbind() const;

            void uniformBool(const std::string& name, bool val);
            void uniformUint(const std::string& name, uint32_t val);
            void uniformInt(const std::string& name, int val);
            void uniformFloat(const std::string& name, float val);
            void uniformFloat2(const std::string& name, const Vec2& val);
            void uniformFloat3(const std::string& name, const Vec3& val);
            void uniformFloat4(const std::string& name, const Vec4& val);
            void uniformMat3(const std::string& name, const Mat3& matrix);
            void uniformMat4(const std::string& name, const Mat4& matrix);

        private:

            uint32_t shaderID;

            /// @brief Reads the file in `filepath` and returns
            ///     the result
            std::string readFile(const std::string& filepath);

            /// @brief Extracts the shaders sources from the file and
            ///     puts them in an unordered map, which is returned
            std::unordered_map<GLenum, std::string> preprocess(const std::string& source);

            /// @brief Compile the map's shaders
            void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    };
}