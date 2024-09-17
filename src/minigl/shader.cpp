#include "core.hpp"
#include "shader.hpp"

namespace minigl
{
    static GLenum shaderTypeFromString(const std::string& type)
    {
        if(type == "vertex") return GL_VERTEX_SHADER;
        if(type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        if(type == "geometry") return GL_GEOMETRY_SHADER;

        MGL_ASSERT(false, "Unknown shader type !");
        return GL_INVALID_ENUM;
    }

    Shader::Shader(const std::string& filepath)
    {
        // Read the source file, extract the shader sources and
        // compile them
        std::string source = readFile(filepath);
        auto shaderSources = preprocess(source);
        compile(shaderSources);

        printf("Loaded shader from file '%s'\n", filepath.c_str());
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
            printf("Could not open file '%s'\n", filepath.c_str());
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> Shader::preprocess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        // Place the cursor at the first "#type" token occurence
        std::string token = "#type";
        size_t tokenLength = token.length();
        size_t pos = source.find(token);

        while (pos != std::string::npos)
        {
            // Place the cursor at the shader type marker word
            size_t begin = pos + tokenLength + 1;
            size_t eol = source.find_first_of("\r\n", pos);
            MGL_ASSERT(eol != std::string::npos, "Error");
            std::string type = source.substr(begin, eol - begin);
            MGL_ASSERT(shaderTypeFromString(type), "Invalid shader type specified.");

            // Add to the unordered map the [shader type] source at
            // [type]
            size_t typePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(token, typePos);
            shaderSources[shaderTypeFromString(type)] = source.substr(typePos, pos - (typePos == std::string::npos ? source.size() - 1 : typePos));
        }

        return shaderSources;
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

                printf("Shader %u compilation failure !", type);
                printf("%s", infoLog.data());
                return;
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

            printf("%s\n", infoLog.data());
            MGL_ASSERT(false, "OpenGLShader link failure !");

            return;
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

    void Shader::uniformBool(const std::string& name, bool val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1i(location, (int)val);
    }

    void Shader::uniformUint(const std::string& name, uint32_t val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1ui(location, val);
    }

    void Shader::uniformInt(const std::string& name, int val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1i(location, val);
    }

    void Shader::uniformFloat(const std::string& name, float val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform1f(location, val);
    }

    void Shader::uniformFloat2(const std::string& name,
                                     const Vec2& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform2f(location, val.x, val.y);
    }

    void Shader::uniformFloat3(const std::string& name, const Vec3& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform3f(location, val.x, val.y, val.z);
    }

    void Shader::uniformFloat4(const std::string& name, const Vec4& val)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniform4f(location, val.x, val.y, val.z, val.w);
    }

    void Shader::uniformMat3(const std::string& name, const Mat3& matrix)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
    }

    void Shader::uniformMat4(const std::string& name, const Mat4& matrix)
    {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }
}