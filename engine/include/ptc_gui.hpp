#ifndef PTC_GUI_HPP
#define PTC_GUI_HPP

#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <vector>

struct Rect {
    int width;
    int height;
    int x;
    int y;
};

struct Panel {
    std::string name;
    Rect rect;
    bool visible = true;
};

class GUI
{
private:
    static GLuint VBO, VAO;
    static Shader shader;
    static std::vector<float> batchedVertices;
    static glm::vec2 cursorPos;
    static void DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color);
public:
    static void InitUI();
    static void RenderUI();

    static void Begin();
    static void End();

    static void Text(std::string _text = "", glm::vec4 _color = glm::vec4(1.0f));
    static void Button(std::string _text = "", std::function<void()> _callback = [](){}, glm::vec4 _color = glm::vec4(1.0f));
    //static void Button(int _x, int _y, int _z, int _w, int _h, std::function<std::any(std::any &&args...)> _onClick, glm::vec4 _color = glm::vec4(0.8f), std::string _text = "Button");
};

#endif