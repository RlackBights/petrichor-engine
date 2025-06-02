#include "ptc_console.hpp"
#include "ptc_font.hpp"
#include "ptc_gui_structs.hpp"
#include "ptc_input.hpp"
#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"
#include "ptc_text.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <glm/fwd.hpp>
#include <memory>
#include <ptc_gui.hpp>
#include <string>
#include <vector>

void GUI::InitUI()
{
    GUIText = Text("");
    GUIText.font = Font::LoadFont("arial.ttf", 20);
    cursorPos = glm::vec2(5, 0);
    shader = Shader("gui_vert.glsl", "gui_frag.glsl");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    GUI::layout = LayoutNode (
        std::make_unique<Split>(
            SplitDirection::SPLIT_VERTICAL,
            0.77f,
            std::make_unique<LayoutNode>(
                std::make_unique<Split>(
                    SplitDirection::SPLIT_HORIZONTAL,
                    0.66,
                    std::make_unique<LayoutNode>(
                        std::make_unique<Split>(
                            SplitDirection::SPLIT_VERTICAL,
                            0.27f,
                            std::make_unique<LayoutNode>(
                                std::make_shared<Panel>("Hierarchy", Rect(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
                            ),
                            std::make_unique<LayoutNode>(
                                std::make_shared<Panel>("Scene", Rect())
                            )
                        )
                    ),
                    std::make_unique<LayoutNode>(
                        std::make_shared<Panel>("Console", Rect(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
                    )
                )
            ),
            std::make_unique<LayoutNode>(
                std::make_shared<Panel>("Inspector", Rect(), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
            )
        )
    );
}
std::shared_ptr<Panel> GUI::GetPanelFromLayout(const std::string& name, LayoutNode& node)
{
    if (std::holds_alternative<std::shared_ptr<Panel>>(node))
    {
        auto panel = std::get<std::shared_ptr<Panel>>(node);
        if (panel->name == name) return panel;
        else return nullptr;
    } else {
        auto& split = *std::get<std::unique_ptr<Split>>(node);
        auto childA = GUI::GetPanelFromLayout(name, *split.childA);
        auto childB = GUI::GetPanelFromLayout(name, *split.childB);
        return (childA) ? childA : childB; 
    }
}
void GUI::ApplyLayout()
{
    GUI::CalculateRects(layout, Renderer::screen);
}
void GUI::CalculateRects(LayoutNode& node, Rect area) {
    if (std::holds_alternative<std::shared_ptr<Panel>>(node)) {
        auto panel = std::get<std::shared_ptr<Panel>>(node);
        panel->rect = area;
        if (panel->name == "Scene") 
        {
            Renderer::viewport = panel->rect;
        } else {
            GUI::DrawQuad(panel->rect.x, panel->rect.y, 0.1, panel->rect.width, panel->rect.height, panel->baseColor, true);
        }
        return;
    }

    auto& split = *std::get<std::unique_ptr<Split>>(node);
    auto& a = split.childA;
    auto& b = split.childB;

    if (!a || !b) return;

    Rect rectA, rectB;

    if (split.direction == SplitDirection::SPLIT_HORIZONTAL) {
        int heightA = static_cast<int>(area.height * split.ratio);
        int heightB = area.height - heightA;

        rectA = Rect(area.x, area.y, area.width, heightA);
        rectB = Rect(area.x, area.y + heightA, area.width, heightB);
    } else {
        int widthA = static_cast<int>(area.width * split.ratio);
        int widthB = area.width - widthA;

        rectA = Rect(area.x, area.y, widthA, area.height);
        rectB = Rect(area.x + widthA, area.y, widthB, area.height);
    }

    CalculateRects(*a, rectA);
    CalculateRects(*b, rectB);
}
void GUI::DrawText(const std::string& text, const glm::vec2& position)
{
    batchedTextEntries.push_back(TextDrawEntry(text, position, (activePanel) ? activePanel->rect : Renderer::screen, activePanel->scrollOffset));
}
void GUI::DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color, bool ignoreOffset)
{
    batchedQuadEntries.push_back(QuadDrawEntry(Rect(_x, _y, _w, _h), (activePanel) ? activePanel->rect : Renderer::screen, _z, _color, (!ignoreOffset) ? &activePanel->scrollOffset : nullptr));
}
void GUI::RenderUI()
{
    for (QuadDrawEntry entry : batchedQuadEntries) {
        float offset = (entry.scrollOffset) ? -*entry.scrollOffset : 0;
        std::vector<float> batchedVertices = {};

        batchedVertices.push_back(entry.quad.x);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - entry.quad.height - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);
        
        batchedVertices.push_back(entry.quad.x + entry.quad.width);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);
        
        batchedVertices.push_back(entry.quad.x);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);

        batchedVertices.push_back(entry.quad.x);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - entry.quad.height - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);
        
        batchedVertices.push_back(entry.quad.x + entry.quad.width);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - entry.quad.height - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);
        
        batchedVertices.push_back(entry.quad.x + entry.quad.width);
        batchedVertices.push_back(Renderer::screen.height - entry.quad.y - offset);
        batchedVertices.push_back(entry.z);
        batchedVertices.push_back(entry.color.r);
        batchedVertices.push_back(entry.color.g);
        batchedVertices.push_back(entry.color.b);
        batchedVertices.push_back(entry.color.a);


        shader.use();
        shader.setFloat2("screenSize", { Renderer::screen.width, Renderer::screen.height });

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glScissor(entry.clipRect.x, Renderer::screen.height - entry.clipRect.y - entry.clipRect.height, entry.clipRect.width, entry.clipRect.height);
        glEnable(GL_SCISSOR_TEST);

        glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), batchedVertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, batchedVertices.size() / 7);

        glBindVertexArray(0);
        glDisable(GL_SCISSOR_TEST);
        
        batchedVertices.clear();
    }
    batchedQuadEntries.clear();

    for (TextDrawEntry entry : batchedTextEntries) {
        GUIText.SetText(entry.text, false);
        entry.position.y += entry.scrollOffset;
        GUIText.ForceDrawText(entry.position, entry.clipRect);
    }
    batchedTextEntries.clear();
    glEnable(GL_DEPTH_TEST);
}
void GUI::wrapFrame()
{
    cursorPos = glm::vec2(100, 0);
}
void GUI::Begin(const std::string& name, const bool scrollable)
{
    if (activePanel) return;
    auto panel = GetPanelFromLayout(name);
    if (panel) {
        activePanel = panel.get();
        cursorPos = glm::vec2(panel->rect.x + 10, panel->rect.y);
    } else {
        cursorPos = glm::vec2(5, 0);
    }
    if (scrollable || panel->rect.height < panel->currentHeight) GUI::Scrollbar();
}
void GUI::End()
{
    if (!activePanel) return;
    activePanel->currentHeight = abs(activePanel->rect.y - cursorPos.y);
    activePanel = nullptr;
    cursorPos = glm::vec2(5, 0);
}
bool GUI::isHovered(const Rect &rect)
{
    return  Input::mouseX >= rect.x &&
            Input::mouseX <= rect.x + rect.width &&
            Input::mouseY >= rect.y && 
            Input::mouseY <= rect.y + rect.height;
}
void GUI::Label(const std::string& text)
{
    DrawText(text, {cursorPos.x, Renderer::screen.height - cursorPos.y - GUIText.font->fontSize});
    cursorPos.y += GUIText.font->fontSize + 5;
}
bool GUI::Button(const std::string &text, const glm::vec2& size)
{
    bool returnVal = false;
    std::size_t id = std::hash<std::string>{}(text);

    glm::vec4 quadCol = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    Rect r = Rect(cursorPos.x, cursorPos.y - activePanel->scrollOffset, size.x, size.y);
    if (GUI::isHovered(activePanel->rect) && GUI::isHovered(r)) 
    {
        quadCol = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        GUI::Hovered = id;
        if (Input::heldMouseButtons[SDLM_LEFT]) {
            GUI::Active = id;
            quadCol = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        } else if (GUI::Active == id && !Input::heldMouseButtons[SDLM_LEFT]) {
            GUI::Active = 0;
            returnVal = true;
        }
    } else if (GUI::Active == id) {
        GUI::Active = 0;
    }

    DrawQuad(cursorPos.x, cursorPos.y, 0, size.x, size.y, quadCol);
    DrawText(text, {cursorPos.x + (size.x - GUIText.getPixelWidth(text)) / 2.0f, Renderer::screen.height - cursorPos.y - (GUIText.font->fontSize + size.y) / 2.0f});

    cursorPos.y += size.y + 5;
    return returnVal;
}
void GUI::Scrollbar(const int width)
{
    activePanel->scrollOffset = std::clamp(activePanel->scrollOffset + Input::mouseScrollRel * 10, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
    DrawQuad(activePanel->rect.x + activePanel->rect.width - width, activePanel->rect.y, -0.1, width, activePanel->rect.height, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), true);
    DrawQuad(activePanel->rect.x + activePanel->rect.width - width * 0.75f, activePanel->rect.y + (activePanel->rect.height - activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight) * (activePanel->scrollOffset / std::max(0.1f, activePanel->currentHeight - activePanel->rect.height)), 0, width * 0.75f, activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), true);
    
}

std::vector<float> GUI::batchedVertices;
std::vector<TextDrawEntry> GUI::batchedTextEntries = {};
std::vector<QuadDrawEntry> GUI::batchedQuadEntries = {};
Shader GUI::shader;
GLuint GUI::VBO, GUI::VAO;
glm::vec2 GUI::cursorPos;
LayoutNode GUI::layout;
std::size_t GUI::Hovered, GUI::Active;
Text GUI::GUIText;
Panel* GUI::activePanel;