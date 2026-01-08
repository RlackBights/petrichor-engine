#pragma once

#include <SDL3/SDL_opengl.h>
#include <string>
namespace RendererBackends::SDL3_OpenGL {
    class Shader
    {
    public:
        GLuint shaderID;

        Shader(const std::string& path, GLint shaderType);
    };
}