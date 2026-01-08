#pragma once

#include "PetrichorRendererAPI/Data/IShaderProgram.h"
#include "SDL3_OpenGL_Backend/Shader.h"
#include <SDL3/SDL_opengl.h>

namespace RendererBackends::SDL3_OpenGL {
    class ShaderProgram : public PetrichorRendererAPI::Data::IShaderProgram
    {
	private:
		GLuint programID = -1;
    public:
		ShaderProgram();
        ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

        void Use();
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetUInt(const std::string& name, unsigned int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat2(const std::string& name, float value[2]) const;
		void SetFloat3(const std::string& name, float value[3]) const;
		void SetFloat4(const std::string& name, float value[4]) const;
		void SetMatrix4x4(const std::string& name, const float* value) const;
    };
}