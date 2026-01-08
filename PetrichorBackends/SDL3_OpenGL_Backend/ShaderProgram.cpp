#include <glad/glad.h>
#include "SDL3_OpenGL_Backend/ShaderProgram.h"

namespace RendererBackends::SDL3_OpenGL {
    ShaderProgram::ShaderProgram()
    {
        programID = -1;
    }
    ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
    {
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader.shaderID);
        glAttachShader(programID, fragmentShader.shaderID);
        glLinkProgram(programID);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(programID);
    }

    void ShaderProgram::SetBool(const std::string& name, bool value) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform1i(uniformLocation, (int)value);
    }

    void ShaderProgram::SetInt(const std::string& name, int value) const
    {
	GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
	if (uniformLocation != -1) glUniform1i(uniformLocation, value);
    }

    void ShaderProgram::SetUInt(const std::string& name, unsigned int value) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform1ui(uniformLocation, value);
    }

    void ShaderProgram::SetFloat(const std::string& name, float value) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform1f(uniformLocation, value);
    }

    void ShaderProgram::SetFloat2(const std::string& name, float value[2]) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform2f(uniformLocation, value[0], value[1]);
    }

    void ShaderProgram::SetFloat3(const std::string& name, float value[3]) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform3f(uniformLocation, value[0], value[1], value[2]);
    }
    
    void ShaderProgram::SetFloat4(const std::string& name, float value[4]) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniform4f(uniformLocation, value[0], value[1], value[2], value[3]);
    }
    
    void ShaderProgram::SetMatrix4x4(const std::string& name, const float* value) const
    {
        GLint uniformLocation = glGetUniformLocation(programID, name.c_str());
        if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value);
    }
}