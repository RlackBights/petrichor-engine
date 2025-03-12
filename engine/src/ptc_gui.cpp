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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
void GUI::DrawQuad(int _x, int _y, int _z, int _w, int _h)
{
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);

    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
}
void GUI::RenderUI()
{
    GLenum err;
    while((err = glGetError())) SDL_Log("%d %d", err, __LINE__);
    shader.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), batchedVertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    batchedVertices.clear();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    while((err = glGetError())) SDL_Log("%d %d", err, __LINE__);
}
void GUI::Panel(int _x, int _y, int _z, int _w, int _h)
{
    DrawQuad(_x, _y, _z, _w, _h);
}
std::vector<float> GUI::batchedVertices;
Shader GUI::shader;
GLuint GUI::VBO, GUI::VAO;