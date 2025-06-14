#ifndef PTC_GUI_HPP
#define PTC_GUI_HPP

#include "ptc_gui_structs.hpp"
#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"
#include <cstddef>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Text;

class GUI
{
private:
    static LayoutNode layout;
    static GLuint VBO, VAO;
    static Shader shader;
    static std::vector<float> batchedVertices;
    static std::vector<TextDrawEntry> batchedTextEntries;
    static std::vector<QuadDrawEntry> batchedQuadEntries;
    
    static glm::vec2 cursorPos;

    static std::size_t Hovered;
    static std::size_t Active;

    static Panel* activePanel;

    static Text GUIText;

    static void SetColors();
    static void DrawText(const std::string& text, const glm::vec2& position, const Rect* clipOverride = nullptr, const bool ignoreOffset = false);
    static void DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color, const bool ignoreOffset = false);
    static void CalculateRects(LayoutNode& node, Rect area);
    static std::shared_ptr<Panel> GetPanelFromLayout(const std::string& name, LayoutNode& node = GUI::layout);
    static void Scrollbar(const int width = 10);
    static void GUISplitter(const Rect& splitter, const SplitDirection direction, float& ratio, const float splitSize);
public:
    static std::map<std::string, glm::vec4> colors;

    static void ApplyLayout();
    static void InitUI();
    static void RenderUI();
    static void wrapFrame();

    static void Begin(const std::string& name, const bool scrollable = false);
    static void End();

    static bool isHovered(const Rect& rect);
    
    static void Divider();
    static void Label(const std::string& text, const bool centered = false);
    static void Title(const std::string& text);
    static bool Button(const std::string& text, const glm::vec2& size, const bool invisible = false);
    static void TextInput(const glm::vec2& size, std::string& text);
};

#endif