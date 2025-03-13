#include "ptc_renderer.h"
#include "ptc_shader.h"
#include <glm/fwd.hpp>
#include <ptc_gui.h>
#include <vector>

void GUI::InitUI()
{
    shader = Shader("gui_vert.glsl", "gui_frag.glsl");
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void GUI::DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color)
{
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);

    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
}
void GUI::RenderUI()
{
    shader.use();
    shader.setFloat2("screenSize", { Renderer::screenWidth, Renderer::screenHeight });
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), batchedVertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, batchedVertices.size());

    batchedVertices.clear();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}
void GUI::Panel(int _x, int _y, int _z, int _w, int _h, glm::vec4 _color)
{
    DrawQuad(_x, _y, _z, _w, _h, _color);
}
std::vector<float> GUI::batchedVertices;
Shader GUI::shader;
GLuint GUI::VBO, GUI::VAO;