#pragma once

#include "GUI/GUIStructs.hpp"
#include "Math/Math.h"
#include "PetrichorRendererAPI/Vertex.h"
#include <map>
#include <vector>

namespace PetrichorEditor::GUI {
    class Text;

    class GUI
    {
    private:
        static LayoutNode layout;
        static std::vector<PetrichorRendererAPI::Vertex> batchedVertices;
        static std::vector<TextDrawEntry> batchedTextEntries;
        static std::vector<QuadDrawEntry> batchedQuadEntries;

        static std::size_t Hovered;
        static std::size_t Active;

        static Panel* activePanel;

        static Text GUIText;

        static void SetColors();
        static void DrawText(const std::string& text, const PetrichorEngine::Math::Vector2& position, const Rect* clipOverride = nullptr, const bool ignoreOffset = false);
        static void CalculateRects(LayoutNode& node, Rect area);
        static std::shared_ptr<Panel> GetPanelFromLayout(const std::string& name, LayoutNode& node = GUI::layout);
        static void Scrollbar(const int width = 10);
        static void GUISplitter(const Rect& splitter, const SplitDirection direction, float& ratio, const float splitSize);
        static std::map<std::string, PetrichorEngine::Math::Vector4> colors;
    
    public:
        static void ApplyLayout();
        static void InitializeGUI();
        static void RenderGUI();
        static void WrapGUI();

        static void Begin(const std::string& name);
        static void Begin(const std::string& name, const bool scrollable = true, const bool hasHeader = true);
        static void End();

        static bool isHovered(const Rect& rect);
        
        static void Divider();
        static void Label(const std::string& text, const bool centered = false);
        static void Title(const std::string& text);
        static bool Button(const std::string& text, const PetrichorEngine::Math::Vector2& size, const bool invisible = false);
        static void TextInput(const PetrichorEngine::Math::Vector2& size, std::string& text);
    };
}
