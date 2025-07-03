#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_Filepath(filepath), m_RendererID(0)
{
    ShaderProgramSource soruce = ParseShader(filepath);
    m_RendererID = CreateShader(soruce.VertexSource, soruce.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}


void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform2fvLoc(int location, glm::vec2 vector)
{
    GLCall(glUniform2fv(location, 1, glm::value_ptr(vector)));
}

void Shader::SetUniform3fvLoc(int location, glm::vec3 vector)
{
    GLCall(glUniform4fv(location, 1, glm::value_ptr(vector)));
}

void Shader::SetUniform4fvLoc(int location, glm::vec4 vector)
{
    GLCall(glUniform4fv(location, 1, glm::value_ptr(vector)));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "WARNING : Uniform " << name << " doesn't exist!" << std::endl;
    }
    
    m_UniformLocationCache[name] = location;

    return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

   /* int result;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(program, length, &length, message));
        std::cout << "Failed to link " << std::endl;
        std::cout << message << std::endl;
        return 0;
    }*/

    // Check link status
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        std::cout << "ERROR: Shader linking failed!\n" << infoLog.data() << std::endl;

        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }


    GLCall(glValidateProgram(program));

    // delete the intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //char message[length]; // we cannot dynamically allocate on stack so instead
        char* message = (char*)malloc(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}