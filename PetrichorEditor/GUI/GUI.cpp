#include <array>
#include <glad/glad.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "GUI/GUI.h"
#include "Math/Math.h"
#include "PetrichorInputAPI/InputManager.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Renderer.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include "SDL3_OpenGL_Backend/Shader.h"
#include "SDL3_OpenGL_Backend/ShaderProgram.h"

namespace PetrichorEditor {
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

    void GUI::InitializeGUI()
    {
        GUI::SetColors();
        GUIFont = PetrichorRendererAPI::Text::FontLoader::LoadFont("Resources/Fonts/arial.ttf", 20);
        cursorPos = PetrichorEngine::Math::Vector2(5, 0);
        shaderProgram = ShaderProgram(Shader("Resources/Shaders/vert.glsl", GL_VERTEX_SHADER), Shader("Resources/Shaders/frag.glsl", GL_FRAGMENT_SHADER));
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

        shaderProgram.Use();

        GL_CHECK_ERROR();
        
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
    int GUI::CalculateTextPixelWidth(const std::string& text, PetrichorRendererAPI::Text::Font* font)
    {
        int width = 0;
        size_t i = 0;

        while (i < text.size())
        {
            char32_t c = 0;
            unsigned char byte = text[i];

            if (byte <= 0x7F)
            {
                c = byte;
                i += 1;
            }
            else if ((byte & 0xE0) == 0xC0)
            {
                c = ((text[i] & 0x1F) << 6) | (text[i + 1] & 0x3F);
                i += 2;
            }
            else if ((byte & 0xF0) == 0xE0)
            {
                c = ((text[i] & 0x0F) << 12) | ((text[i + 1] & 0x3F) << 6) | (text[i + 2] & 0x3F);
                i += 3;
            }
            else if ((byte & 0xF8) == 0xF0)
            {
                c = ((text[i] & 0x07) << 18) | ((text[i + 1] & 0x3F) << 12) | ((text[i + 2] & 0x3F) << 6) | (text[i + 3] & 0x3F);
                i += 4;
            }

            PetrichorRendererAPI::Text::Character ch = font->GetCharacter(c);
            width += ch.Advance;
        }

        return width;
    }
    void GUI::ApplyLayout()
    {
        GUI::CalculateRects(layout, PetrichorRendererAPI::Renderer::GetScreenRect());
    }

    void GUI::CalculateRects(LayoutNode& node, Rect area) {
        if (std::holds_alternative<std::shared_ptr<Panel>>(node)) {
            auto panel = std::get<std::shared_ptr<Panel>>(node);
            panel->rect = area;
            if (panel->name == "Scene") 
            {
                std::cout << "[ERROR] Rendering Viewport not implemented yet!\r\n";
                // Renderer::viewport = panel->rect;
            } else {
                GUI::DrawRect(Rect(panel->rect.x, panel->rect.y, panel->rect.width, panel->rect.height), 1, (panel->baseColor.w == -1.0f) ? GUI::colors["background"] : panel->baseColor, true);
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

    void GUI::DrawText(const std::string& text, const glm::vec2& position, const Rect* clipOverride, const bool ignoreOffset)
    {
        Rect screenRect = PetrichorRendererAPI::Renderer::GetScreenRect();
        // Skip offscreen elements
        if ((!ignoreOffset && (position.y + activePanel->scrollOffset < 0 || position.y + activePanel->scrollOffset > screenRect.height))
            || (ignoreOffset && (position.y < 0 || position.y > screenRect.height))
            || position.x < 0 || position.x > screenRect.width) return;

        batchedTextEntries.push_back(TextDrawEntry(text, position, (clipOverride) ? *clipOverride : ((activePanel) ? activePanel->rect : screenRect), ((!ignoreOffset) ? activePanel->scrollOffset : 0)));
    }

    void GUI::DrawRect(const Rect& rect, int z, PetrichorEngine::Math::Vector4 _color, const bool ignoreOffset)
    {
        Rect screenRect = PetrichorRendererAPI::Renderer::GetScreenRect();
        // Skip offscreen elements
        if ((!ignoreOffset && (rect.y + activePanel->scrollOffset < 0 || rect.y + activePanel->scrollOffset > screenRect.height))
            || (ignoreOffset && (rect.y < 0 || rect.y > screenRect.height))
            || rect.x < 0 || rect.x > screenRect.width) return;
        batchedRectEntries.push_back(RectDrawEntry(Rect(rect.x, rect.y, rect.width, rect.height), z, (activePanel) ? activePanel->rect : screenRect, _color, (!ignoreOffset) ? &activePanel->scrollOffset : nullptr));
    }

    void GUI::RenderGUI()
    {
        GLint polygonMode;
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);GL_CHECK_ERROR();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);GL_CHECK_ERROR();
        Rect screen = PetrichorRendererAPI::Renderer::GetScreenRect();
        for (RectDrawEntry entry : batchedRectEntries) {
            float offset = (entry.scrollOffset) ? -*entry.scrollOffset : 0;
            std::vector<float> batchedVertices = {};

            batchedVertices.push_back(entry.rect.x);
            batchedVertices.push_back(screen.height - entry.rect.y - entry.rect.height - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);
            
            batchedVertices.push_back(entry.rect.x + entry.rect.width);
            batchedVertices.push_back(screen.height - entry.rect.y - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);
            
            batchedVertices.push_back(entry.rect.x);
            batchedVertices.push_back(screen.height - entry.rect.y - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);

            batchedVertices.push_back(entry.rect.x);
            batchedVertices.push_back(screen.height - entry.rect.y - entry.rect.height - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);
            
            batchedVertices.push_back(entry.rect.x + entry.rect.width);
            batchedVertices.push_back(screen.height - entry.rect.y - entry.rect.height - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);
            
            batchedVertices.push_back(entry.rect.x + entry.rect.width);
            batchedVertices.push_back(screen.height - entry.rect.y - offset);
            batchedVertices.push_back(entry.z);
            batchedVertices.push_back(entry.color.r);
            batchedVertices.push_back(entry.color.g);
            batchedVertices.push_back(entry.color.b);
            batchedVertices.push_back(entry.color.a);
            
            shaderProgram.Use();
            
            float size[] = {(float)screen.width, (float)screen.height};
            shaderProgram.SetFloat2("screenSize", size); GL_CHECK_ERROR();
            // shaderProgram.SetFloat2("quadPos", new float[]{ (float)entry.rect.x, (float)PetrichorRendererAPI::Renderer::GetScreenRect().height - entry.rect.y - entry.rect.height });GL_CHECK_ERROR();
            // shaderProgram.SetFloat2("quadSize", new float[]{ (float)entry.rect.width, (float)entry.rect.height });GL_CHECK_ERROR();

            // glEnable(GL_DEPTH_TEST);GL_CHECK_ERROR();
            glBindVertexArray(VAO);GL_CHECK_ERROR();
            glBindBuffer(GL_ARRAY_BUFFER, VBO);GL_CHECK_ERROR();
            // glViewport(screen.x, screen.y, screen.width, screen.height);GL_CHECK_ERROR();
            // glScissor(entry.clipRect.x, screen.height - entry.clipRect.y - entry.clipRect.height, entry.clipRect.width, entry.clipRect.height);GL_CHECK_ERROR();
            // glEnable(GL_SCISSOR_TEST);GL_CHECK_ERROR();

            glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), batchedVertices.data(), GL_DYNAMIC_DRAW);GL_CHECK_ERROR();
            glDrawArrays(GL_TRIANGLES, 0, batchedVertices.size() / 7);GL_CHECK_ERROR();

            glBindVertexArray(0);GL_CHECK_ERROR();
            glDisable(GL_SCISSOR_TEST); GL_CHECK_ERROR();
            
            batchedVertices.clear();
        }
        batchedRectEntries.clear();

        for (TextDrawEntry entry : batchedTextEntries) {
            //GUIText.SetText(entry.text, false);
            // entry.position.y += entry.scrollOffset;
            // PetrichorRendererAPI::Renderer::DrawText(entry.text, GUIFont, Material());
            //GUIText.ForceDrawText(entry.position, entry.clipRect);
        }
        batchedTextEntries.clear();
        
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }

    void GUI::WrapGUI()
    {
        if (GUI::Hovered == 0) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_DEFAULT);
        GUI::Hovered = 0;
        cursorPos = glm::vec2(100, 0);
    }
    void GUI::Begin(const std::string& name, const bool scrollable, const bool hasHeader)
    {
        if (activePanel) return;
        auto panel = GetPanelFromLayout(name);
        if (panel) {
            activePanel = panel.get();
            cursorPos = glm::vec2(panel->rect.x + 10, panel->rect.y);
        } else {
            cursorPos = glm::vec2(5, 0);
        }
        if (hasHeader)
        {
            GUI::Title(name);
            GUI::Divider();
        }
        if (scrollable || panel->rect.height < panel->currentHeight) GUI::Scrollbar();
    }
    void GUI::End()
    {
        if (!activePanel) return;
        activePanel->currentHeight = std::abs(activePanel->rect.y - cursorPos.y);
        if (activePanel->currentHeight <= activePanel->rect.height) activePanel->scrollOffset = 0;
        activePanel = nullptr;
        cursorPos = glm::vec2(5, 0);
    }
    bool GUI::isHovered(const Rect &rect)
    {
        return  PetrichorInputAPI::InputManager::GetMousePosition()[0] >= rect.x &&
                PetrichorInputAPI::InputManager::GetMousePosition()[0] <= rect.x + rect.width &&
                PetrichorInputAPI::InputManager::GetMousePosition()[1] >= rect.y && 
                PetrichorInputAPI::InputManager::GetMousePosition()[1] <= rect.y + rect.height;
    }
    void GUI::Divider()
    {
        DrawRect(Rect(activePanel->rect.x, cursorPos.y - ((activePanel) ? activePanel->scrollOffset : 0), activePanel->rect.width, 2), 0, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), true);
        cursorPos.y += 7;
    }

    void GUI::Label(const std::string& text, const bool centered)
    {
        DrawText(text, {cursorPos.x + ((centered) ? (activePanel->rect.width - CalculateTextPixelWidth(text, GUIFont)) / 2 : 0), PetrichorRendererAPI::Renderer::GetScreenRect().height - cursorPos.y - GUIFont->fontSize});
        cursorPos.y += GUIFont->fontSize + 5;
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
            if (PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT) && GUI::Active == 0) {
                GUI::Active = id;
            } else if (GUI::Active == id && !PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
                GUI::Active = 0;
                returnVal = true;
            } else if (GUI::Active == id) {
                quadCol = colors["buttonPressed"];
            }
        } else if (GUI::Active == id) {
            GUI::Active = 0;
        }

        if (hovered) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_POINTER);

        if (!invisible) DrawRect(Rect(cursorPos.x, cursorPos.y - activePanel->scrollOffset, size.x, size.y), 0, quadCol, true);
        DrawText(text, {cursorPos.x + (size.x - CalculateTextPixelWidth(text, GUIFont)) / 2.0f, PetrichorRendererAPI::Renderer::GetScreenRect().height - cursorPos.y - (GUIFont->fontSize + size.y) / 2.0f});

        cursorPos.y += size.y + 5;
        return returnVal;
    }
    void GUI::Scrollbar(const int width)
    {
        std::size_t id = std::hash<std::string>{}(activePanel->name);
        glm::vec4 quadCol = colors["buttonNormal"];
        bool hovered = isHovered(Rect(activePanel->rect.x + activePanel->rect.width - width, activePanel->rect.y, width, activePanel->rect.height));

        if (hovered)
        {
            quadCol = colors["buttonHovered"];
            GUI::Hovered = id;
            if (GUI::Active == 0 && PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
                GUI::Active = id;
            }
            activePanel->scrollOffset = std::clamp(activePanel->scrollOffset + PetrichorInputAPI::InputManager::GetMouseScroll() * 50, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
        } else {
            activePanel->scrollOffset = std::clamp(activePanel->scrollOffset + PetrichorInputAPI::InputManager::GetMouseScroll() * 10, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
        }

        if (GUI::Active == id)
        {
            if (!PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
                GUI::Active = 0;
            }
            quadCol = colors["buttonPressed"];
            activePanel->scrollOffset = std::clamp((float)std::clamp((int)(PetrichorInputAPI::InputManager::GetMousePosition()[1] - activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight / 2) - activePanel->rect.y, 0, activePanel->rect.height) / activePanel->rect.height * activePanel->currentHeight, 0.0f, std::max(0.0f, activePanel->currentHeight - activePanel->rect.height));
        }

        if (hovered) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_POINTER);

        DrawRect(Rect(activePanel->rect.x + activePanel->rect.width - width, activePanel->rect.y, width, activePanel->rect.height), -1, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), true);
        DrawRect(Rect(activePanel->rect.x + activePanel->rect.width - width * 0.75f, activePanel->rect.y + (activePanel->rect.height - activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight) * (activePanel->scrollOffset / std::max(0.1f, activePanel->currentHeight - activePanel->rect.height)), width * 0.75f, activePanel->rect.height * activePanel->rect.height / activePanel->currentHeight), -2, quadCol, true);
    }

    void GUI::TextInput(const glm::vec2 &size, std::string &text)
    {
        std::size_t id = std::hash<std::string>{}(GUI::FormatString("TI%f.2%s", cursorPos.y, activePanel->name.c_str()));
        bool hovered = isHovered(activePanel->rect) && isHovered(Rect(cursorPos.x, cursorPos.y, size.x, size.y));
        if (hovered) GUI::Hovered = id;
        if (hovered && PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) GUI::Active = id;
        else if (!hovered && PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT) && GUI::Active == id) GUI::Active = 0;

        glm::vec4 quadCol = colors["buttonNormal"];
        if (GUI::Active == id) quadCol = colors["buttonPressed"];
        else if (hovered) quadCol = colors["buttonHovered"];

        if (GUI::Active == id && std::string(PetrichorInputAPI::InputManager::GetLastCharacter()) != "") text.append(PetrichorInputAPI::InputManager::GetLastCharacter());
        else if (GUI::Active == id && PetrichorInputAPI::InputManager::GetLastKey() == SDLK_BACKSPACE)
        {
            if (PetrichorInputAPI::InputManager::IsKeyDown(SDLK_LCTRL, nullptr) || PetrichorInputAPI::InputManager::IsKeyDown(SDLK_RCTRL, nullptr) ) {
                bool startRemoving = false;
                int i = text.size() - 1;
                for (; i > 0; i--) {
                    if (text[i] == ' ' && startRemoving) break;
                    else if (text[i] != ' ') startRemoving = true;
                }
                text = text.substr(0, i);
            } else {
                size_t i = text.size();
                while (i > 0 && (static_cast<unsigned char>(text[--i]) & 0xC0) == 0x80);
                text.erase(i);
            }
        }

        if (hovered) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_POINTER);

        DrawRect(Rect(cursorPos.x, cursorPos.y, std::max(size.x, (float)CalculateTextPixelWidth(text, GUIFont)) + 10, size.y), 0, quadCol);
        DrawText(text, {cursorPos.x + 5, PetrichorRendererAPI::Renderer::GetScreenRect().height - cursorPos.y - (GUIFont->fontSize + size.y) / 2.0f});
        cursorPos.y += size.y + 5;
    }

    void GUI::GUISplitter(const Rect& splitter, const SplitDirection direction, float& ratio, const float splitSize)
    {
        std::size_t id = std::hash<std::string>{}(FormatString("SP%d%d", splitter.width, splitter.height));

        bool hovered = GUI::isHovered(splitter);
        
        if (hovered || (GUI::Active == id && PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT))) {
            GUI::Hovered = id;
            if (GUI::Active == 0 && PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
                GUI::Active = id;
            }
            if (GUI::Active == id) {
                if (direction == SplitDirection::SPLIT_HORIZONTAL) {
                    ratio = std::clamp((PetrichorInputAPI::InputManager::GetMousePosition()[1] / splitSize), 0.1f, 0.9f);
                } else {
                    ratio = std::clamp((PetrichorInputAPI::InputManager::GetMousePosition()[0] / splitSize), 0.1f, 0.9f);
                }
            }
        } else if (GUI::Active == id && !PetrichorInputAPI::InputManager::IsMouseButtonDown(SDL_BUTTON_LEFT)) {
            GUI::Active = 0;
        }

        if (hovered && direction == SplitDirection::SPLIT_HORIZONTAL) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_NS_RESIZE);
        else if (hovered) PetrichorRendererAPI::Renderer::SetCursorIcon(SDL_SYSTEM_CURSOR_EW_RESIZE);
        
        DrawRect(Rect(splitter.x, splitter.y, splitter.width, splitter.height), -2, colors["splitter"], true);
    }

    std::vector<PetrichorRendererAPI::Data::Vertex> GUI::batchedVertices;
    std::vector<TextDrawEntry> GUI::batchedTextEntries = {};
    std::vector<RectDrawEntry> GUI::batchedRectEntries = {};
    ShaderProgram GUI::shaderProgram;
    GLuint GUI::VBO, GUI::VAO;
    PetrichorEngine::Math::Vector2 GUI::cursorPos;
    LayoutNode GUI::layout;
    std::size_t GUI::Hovered, GUI::Active;
    Panel* GUI::activePanel;
    std::map<std::string, glm::vec4> GUI::colors;
    PetrichorRendererAPI::Text::Font* GUI::GUIFont;
}

