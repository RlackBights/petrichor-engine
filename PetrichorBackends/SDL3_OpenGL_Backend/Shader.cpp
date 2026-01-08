#include <glad/glad.h>
#include "SDL3_OpenGL_Backend/Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace RendererBackends::SDL3_OpenGL {
    Shader::Shader(const std::string& path, GLint shaderType)
    {
		std::ifstream fileStream;
		std::stringstream stringStream;
        shaderID = -1;

		fileStream.exceptions(std::ifstream::badbit);

        try
		{
			fileStream.open(path);

			stringStream << fileStream.rdbuf();

			fileStream.close();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "[ERROR] Error reading shader: " << e.what() << "\r\n";
		}
		
		std::string shaderContents = stringStream.str();
		const char* primitiveShaderContents = shaderContents.c_str();

        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &primitiveShaderContents, nullptr);
        glCompileShader(shaderID);
    }
}