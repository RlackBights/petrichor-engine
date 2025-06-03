#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
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
#include <map>
#include <memory>
#include <ptc_gui.hpp>
#include <string>
#include <vector>

void GUI::SetColors()
{
    GUI::colors = {
        {"splitter", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},  
        {"background", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)},  
        {"buttonNormal", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)},
        {"buttonHovered", glm::vec4(0.35f, 0.35f, 0.35f, 1.0f)},
        {"buttonPressed", glm::vec4(0.25f, 0.25f, 0.25f, 1.0f)},
        {"scrollbarNormal", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)},
        {"scrollbarHovered", glm::vec4(0.55f, 0.55f, 0.55f, 1.0f)},
        {"scrollbarPressed", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)},
    };
}
void GUI::InitUI()
{
    GUI::SetColors();
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
                                std::make_shared<Panel>("Hierarchy", Rect())
                            ),
                            std::make_unique<LayoutNode>(
                                std::make_shared<Panel>("Scene", Rect())
                            )
                        )
                    ),
                    std::make_unique<LayoutNode>(
                        std::make_shared<Panel>("Console", Rect())
                    )
                )
            ),
            std::make_unique<LayoutNode>(
                std::make_shared<Panel>("Inspector", Rect())
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
            GUI::DrawQuad(panel->rect.x, panel->rect.y, 0.1, panel->rect.width, panel->rect.height, (panel->baseColor.w == -1.0f) ? GUI::colors["background"] : panel->baseColor, true);
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

        GUI::GUISplitter(Rect(area.x, area.y + heightA - 1, area.width, 2), SplitDirection::SPLIT_HORIZONTAL, split.ratio, area.height);
    } else {
        int widthA = static_cast<int>(area.width * split.ratio);
        int widthB = area.width - widthA;

        rectA = Rect(area.x, area.y, widthA, area.height);
        rectB = Rect(area.x + widthA, area.y, widthB, area.height);

        GUI::GUISplitter(Rect(area.x + widthA - 1, area.y, 2, area.height), SplitDirection::SPLIT_VERTICAL, split.ratio, area.width);
    }

    CalculateRects(*a, rectA);
    CalculateRects(*b, rectB);
}
void GUI::DrawText(const std::string& text, const glm::vec2& position, const Rect* clipOverride)
{
    batchedTextEntries.push_back(TextDrawEntry(text, position, (clipOverride) ? *clipOverride : ((activePanel) ? activePanel->rect : Renderer::screen), activePanel->scrollOffset));
}
void GUI::DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color, bool ignoreOffset)
{
    batchedQuadEntries.push_back(QuadDrawEntry(Rect(_x, _y, _w, _h), (activePanel) ? activePanel->rect : Renderer::screen, _z, _color, (!ignoreOffset) ? &activePanel->scrollOffset : nullptr));
}
void GUI::RenderUI()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        shader.setFloat2("border", { 0, 0 });
        shader.setFloat2("quadPos", { entry.quad.x, Renderer::screen.height - entry.quad.y - entry.quad.height });
        shader.setFloat2("quadSize", { entry.quad.width, entry.quad.height });

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glViewport(Renderer::screen.x, Renderer::screen.y, Renderer::screen.width, Renderer::screen.height);
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
    glPolygonMode(GL_FRONT_AND_BACK, Renderer::renderMode);
}
void GUI::wrapFrame()
{
    if (GUI::Hovered != 0) Renderer::SetCursor(SDL_SYSTEM_CURSOR_POINTER);
    else Renderer::SetCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    GUI::Hovered = 0;
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
	GUI::Title(name);
	GUI::Divider();
    if (scrollable || panel->rect.height < panel->currentHeight) GUI::Scrollbar();
}
void GUI::End()
{
    if (!activePanel) return;
    activePanel->currentHeight = abs(activePanel->rect.y - cursorPos.y);
    if (activePanel->currentHeight <= activePanel->rect.height) activePanel->scrollOffset = 0;
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
void GUI::Divider()
{
    DrawQuad(activePanel->rect.x, cursorPos.y, 0, activePanel->rect.width, 2, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    cursorPos.y += 7;
}
void GUI::Label(const std::string& text, const bool centered)
{
    DrawText(text, {cursorPos.x + ((centered) ? (activePanel->rect.width - GUIText.getPixelWidth(text)) / 2 : 0), Renderer::screen.height - cursorPos.y - GUIText.font->fontSize});
    cursorPos.y += GUIText.font->fontSize + 5;
}
void GUI::Title(const std::string& text)
{
    GUI::Label(text, true);
}
bool GUI::Button(const std::string &text, const glm::vec2& size, const bool invisible)
{
    bool returnVal = false;
    std::size_t id = std::hash<std::string>{}(text);
    Rect r = Rect(cursorPos.x, cursorPos.y - activePanel->scrollOffset, size.x, size.y);
    bool hovered = GUI::isHovered(activePanel->rect) && GUI::isHovered(r);

    glm::vec4 quadCol = colors["buttonNormal"];

    if (hovered) 
    {
        quadCol = colors["buttonHovered"];
        GUI::Hovered = id;
        if (Input::heldMouseButtons[SDLM_LEFT] && GUI::Active == 0) {
            GUI::Active = id;
        } else if (GUI::Active == id && !Input::heldMouseButtons[SDLM_LEFT]) {
            GUI::Active = 0;
            returnVal = true;
        } else if (GUI::Active == id) {
            quadCol = colors["buttonPressed"];
        }
    } else if (GUI::Active == id) {
        GUI::Active = 0;
    }

    if (!invisible) DrawQuad(cursorPos.x, cursorPos.y, 0, size.x, size.y, quadCol);
    DrawText(text, {cursorPos.x + (size.x - GUIText.getPixelWidth(text)) / 2.0f, Renderer::screen.height - cursorPos.y - (GUIText.font->fontSize + size.y) / 2.0f});

    cursorPos.y += size.y + 5;
    return returnVal;
}
void GUI::Scrollbar(const int width)
{
    std::size_t id = std::hash<std::string>{}(activePanel->name);
    glm::vec4 quadCol = colors["buttonNormal"];

    if (isHovered(activePanel->rect))
    {
        if (isHovered(Rect(activePanel->rect.x + activePanel->rect.width - width, activePanel->rect.y, width, activePanel->rect.height)))
        {
            quadCol = colors["buttonHovered"];
            GUI::Hovered = id;
            if (GUI::Active == 0 && Input::heldMouseButtons[SDLM_LEFT]) {
                GUI::Active = id;
            }
            activePanel->scrollOffset = std::clamp(activePanel->scrollOffset + Input::mouseScrollRel * 50, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
        } else {
            activePanel->scrollOffset = std::clamp(activePanel->scrollOffset + Input::mouseScrollRel * 10, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
        }
    }

    if (GUI::Active == id)
    {
        if (!Input::heldMouseButtons[SDLM_LEFT]) {
            GUI::Active = 0;
        }
        quadCol = colors["buttonPressed"];
        activePanel->scrollOffset = std::clamp((float)std::clamp((int)(Input::mouseY - activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight / 2) - activePanel->rect.y, 0, activePanel->rect.height) / activePanel->rect.height * activePanel->currentHeight, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
    }

    DrawQuad(activePanel->rect.x + activePanel->rect.width - width, activePanel->rect.y, -0.1, width, activePanel->rect.height, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), true);
    DrawQuad(activePanel->rect.x + activePanel->rect.width - width * 0.75f, activePanel->rect.y + (activePanel->rect.height - activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight) * (activePanel->scrollOffset / std::max(0.1f, activePanel->currentHeight - activePanel->rect.height)), -0.2, width * 0.75f, activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight, quadCol, true);
}
void GUI::TextInput(const glm::vec2 &size, std::string &text)
{
    std::size_t id = std::hash<std::string>{}(Console::FormatString("TI%f.2%s", cursorPos.y, activePanel->name.c_str()));
    bool hovered = isHovered(activePanel->rect) && isHovered(Rect(cursorPos.x, cursorPos.y, size.x, size.y));
    if (hovered) GUI::Hovered = id;
    if (hovered && Input::heldMouseButtons[SDLM_LEFT]) GUI::Active = id;
    else if (!hovered && Input::heldMouseButtons[SDLM_LEFT] && GUI::Active == id) GUI::Active = 0;

    glm::vec4 quadCol = colors["buttonNormal"];
    if (GUI::Active == id) quadCol = colors["buttonPressed"];
    else if (hovered) quadCol = colors["buttonHovered"];

    if (GUI::Active == id && Input::lastCharacter != "") text.append(Input::lastCharacter);
    else if (GUI::Active == id && Input::lastKeyDown == SDLK_BACKSPACE)
    {
        if (Input::getKey(SDLK_LCTRL) || Input::getKey(SDLK_RCTRL) ) {
            bool startRemoving = false;
            int i = text.size() - 1;
            for (; i > 0; i--) {
                if (text[i] == ' ' && startRemoving) break;
                else if (text[i] != ' ') startRemoving = true;
            }
            Console::WriteLine(i);
            text = text.substr(0, i);
        } else {
            size_t i = text.size();
            while (i > 0 && (static_cast<unsigned char>(text[--i]) & 0xC0) == 0x80);
            text.erase(i);
        }
    }

    DrawQuad(cursorPos.x, cursorPos.y, 0, std::max(size.x, (float)GUIText.getPixelWidth(text)) + 10, size.y, quadCol);
    DrawText(text, {cursorPos.x + 5, Renderer::screen.height - cursorPos.y - (GUIText.font->fontSize + size.y) / 2.0f});
    cursorPos.y += size.y + 5;
}
void GUI::GUISplitter(const Rect& splitter, const SplitDirection direction, float& ratio, const float splitSize)
{
    std::size_t id = std::hash<std::string>{}(Console::FormatString("%d%d", splitter.width, splitter.height));

    bool hovered = GUI::isHovered(splitter);
    
    if (hovered || (GUI::Active == id && Input::heldMouseButtons[SDLM_LEFT])) {
        GUI::Hovered = id;
        if (GUI::Active == 0 && Input::heldMouseButtons[SDLM_LEFT]) {
            GUI::Active = id;
        }
        if (GUI::Active == id) {
            if (direction == SplitDirection::SPLIT_HORIZONTAL) {
                ratio = std::clamp((Input::mouseY / splitSize), 0.1f, 0.9f);
            } else {
                ratio = std::clamp((Input::mouseX / splitSize), 0.1f, 0.9f);
            }
        }
    } else if (GUI::Active == id && !Input::heldMouseButtons[SDLM_LEFT]) {
        GUI::Active = 0;
    }
    
    DrawQuad(splitter.x, splitter.y, -0.2, splitter.width, splitter.height, colors["splitter"], true);
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
std::map<std::string, glm::vec4> GUI::colors;