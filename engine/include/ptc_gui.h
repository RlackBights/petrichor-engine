#ifndef PTC_GUI_H
#define PTC_GUI_H

#include "ptc_shader.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <vector>

class GUI
{
private:
    static GLuint VBO, VAO;
    static Shader shader;
    static std::vector<float> batchedVertices;
    static void DrawQuad(int _x, int _y, int _z, int _w, int _h);
public:
    static void InitUI();
    static void RenderUI();

    static void Panel(int _x, int _y, int _z, int _w, int _h);
};

#endif