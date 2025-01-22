#include "core.hpp"
#include "shader.hpp"

namespace minigl
{
    std::string to_string(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::VERTEX: return "VERTEX";
            case ShaderType::FRAGMENT: return "FRAGMENT";
            case ShaderType::GEOMETRY: return "GEOMETRY";
            case ShaderType::COMPUTE: return "COMPUTE";
        }

        return "UNKNOWN";
    }

    Shader::Shader(const std::string& filepath)
    {
        // Read the source file, extract the shader sources and
        // compile them
        std::string source = readFile(filepath);
        auto shaderSources = preprocess(source);
        compile(shaderSources);

        trace("Loaded shader from file '{}'", filepath);
    }

    Shader::Shader(const std::string& vertexSrc,
                   const std::string& fragmentSrc)
    {
        // Compile the shaders from their sources
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(sources);
    }

    Shader::~Shader()
    {
        glDeleteProgram(shaderID);
    }

    std::string Shader::readFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream file(filepath, std::ios::binary);

        if(file)
        {
            // Make 'result' the size of the file string
            file.seekg(0, std::ios::end);
            result.resize(file.tellg());

            // Read the file from the beggining to result.size()
            // (that is, entirely) and place it in 'result'.
            file.seekg(0, std::ios::beg);
            file.read(&result[0], result.size());

            file.close();
        }
        else
        {
            error("Could not open file '{}'", filepath);
            MGL_ASSERT(false, "Error in reading shader.");
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> Shader::preprocess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shader_sources;

        auto add_define = [&shader_sources](std::string source, ShaderType type)
        {            
            std::string version = "#version 430 core\n";
            std::string define = "#define " + to_string(type) + "\n";
         
            // Only add the shader to the sources if it is
            // actually present in the source.
            size_t type_pos = source.find("#ifdef " + to_string(type));
            if (type_pos != std::string::npos) {
                source.insert(0, version + define);   
                shader_sources[GLenum(type)] = source;
            }
        };

        add_define(source, ShaderType::VERTEX);
        add_define(source, ShaderType::FRAGMENT);
        add_define(source, ShaderType::GEOMETRY);
        add_define(source, ShaderType::COMPUTE);

        return shader_sources;
    }

    void Shader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        shaderID = glCreateProgram();

        MGL_ASSERT(shaderSources.size() <= 3, "Too many shaders.");
        std::array<GLenum, 3> glShaderIDs {};

        // Compile the shaders
        int shaderIndex = 0;
        for (auto&[type, source]: shaderSources)
        {
            GLuint shader = glCreateShader(type);
            const char* source_c_str = source.c_str();

            glShaderSource(shader, 1, &source_c_str, nullptr);
            glCompileShader(shader);

            // Check if the compilation fails
            int isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            if(isCompiled == GL_FALSE)
            {
                int maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<char> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                error("Shader {} compilation failure !", type);
                error("{}", infoLog.data());
                MGL_ASSERT(false, "OpenGL shader compilation failure !");
            }

            glAttachShader(shaderID, shader);
            glShaderIDs[shaderIndex++] = shader;
        }

        // Link our bufferID
        glLinkProgram(shaderID);

        int isLinked = 0;
        glGetProgramiv(shaderID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(shaderID);

            for (auto& id: glShaderIDs)
                glDeleteShader(id);

            error("Shader link failure !");
            error("{}", infoLog.data());
            MGL_ASSERT(false, "OpenGL shader link failure !");
        }

        for (auto& id: glShaderIDs)
        {
            // Always detach the shaders after a succesful linkage
            glDetachShader(shaderID, id);
        }
    }

    void Shader::bind() const
    {
        glUseProgram(shaderID);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    void Shader::upload(const std::string& name, bool val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform1i(shaderID, location, (int)val);
    }

    void Shader::upload(const std::string& name, uint32_t val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform1ui(shaderID, location, val);
    }

    void Shader::upload(const std::string& name, int val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform1i(shaderID, location, val);
    }

    void Shader::upload(const std::string& name, float val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform1f(shaderID, location, val);
    }

    void Shader::upload(const std::string& name, const Vec2& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform2f(shaderID, location, val.x, val.y);
    }

    void Shader::upload(const std::string& name, const Vec3& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform3f(shaderID, location, val.x, val.y, val.z);
    }

    void Shader::upload(const std::string& name, const Vec4& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform4f(shaderID, location, val.x, val.y, val.z, val.w);
    }

    void Shader::upload(const std::string& name, const Mat3& matrix)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniformMatrix3fv(shaderID, location, 1, GL_FALSE, &matrix[0][0]);
    }

    void Shader::upload(const std::string& name, const Mat4& matrix)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniformMatrix4fv(shaderID, location, 1, GL_FALSE, &matrix[0][0]);
    }

    void Shader::texture(const std::string& name, Ref<Texture> texture, uint32_t slot)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glProgramUniform1i(shaderID, location, slot);
    }
}