#ifndef PTC_GUI_HPP
#define PTC_GUI_HPP

#include "ptc_layout_structs.hpp"
#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <vector>

class GUI
{
private:
    static LayoutNode layout;
    static GLuint VBO, VAO;
    static Shader shader;
    static std::vector<float> batchedVertices;
    
    static glm::vec2 cursorPos;
    static int Hovered;
    static int Active;

    static void DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color);
    static void CalculateRects(LayoutNode& node, Rect area);
    static bool isHovered();
public:
    static void ApplyLayout();
    static void InitUI();
    static void RenderUI();

    static void Begin();
    static void End();

    static void Label(const std::string& _text);
    static bool Button(const std::string& _text);
    //static void Button(int _x, int _y, int _z, int _w, int _h, std::function<std::any(std::any &&args...)> _onClick, glm::vec4 _color = glm::vec4(0.8f), std::string _text = "Button");
};

#endif