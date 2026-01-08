#pragma once

#include "GUI/GUIStructs.hpp"
#include "Math/Math.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Data/Vertex.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "SDL3_OpenGL_Backend/ShaderProgram.h"
#include <map>
#include <vector>

namespace PetrichorEditor {
    using namespace RendererBackends::SDL3_OpenGL;
    using namespace PetrichorRendererAPI::Data;

    class GUI
    {
    private:
        static LayoutNode layout;
        static std::vector<PetrichorRendererAPI::Data::Vertex> batchedVertices;
        static std::vector<TextDrawEntry> batchedTextEntries;
        static std::vector<RectDrawEntry> batchedRectEntries;
        static ShaderProgram shaderProgram;
        static GLuint VAO, VBO;

        static std::size_t Hovered;
        static std::size_t Active;

        static PetrichorEngine::Math::Vector2 cursorPos;

        static Panel* activePanel;

        static PetrichorRendererAPI::Text::Font* GUIFont;

        template<typename ... Args>
        static std::string FormatString( const std::string& format, Args ... args)
        {   // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf licensed under CCO 1.0
            int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
            if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
            auto size = static_cast<size_t>( size_s );
            std::unique_ptr<char[]> buf( new char[ size ] );
            std::snprintf( buf.get(), size, format.c_str(), args ... );
            return std::string( buf.get(), buf.get() + size - 1 );
        }

        static void SetColors();
        static void DrawText(const std::string& text, const PetrichorEngine::Math::Vector2& position, const PetrichorRendererAPI::Data::Rect* clipOverride = nullptr, const bool ignoreOffset = false);
        static void DrawRect(const Rect& rect, int z, PetrichorEngine::Math::Vector4 _color, const bool ignoreOffset = false);
        static void CalculateRects(LayoutNode& node, PetrichorRendererAPI::Data::Rect area);
        static int CalculateTextPixelWidth(const std::string& text, PetrichorRendererAPI::Text::Font* font);
        static std::shared_ptr<Panel> GetPanelFromLayout(const std::string& name, LayoutNode& node = GUI::layout);
        static void Scrollbar(const int width = 10);
        static void GUISplitter(const PetrichorRendererAPI::Data::Rect& splitter, const SplitDirection direction, float& ratio, const float splitSize);
        static std::map<std::string, PetrichorEngine::Math::Vector4> colors;
    
    public:
        static void ApplyLayout();
        static void InitializeGUI();
        static void RenderGUI();
        static void WrapGUI();

        static void Begin(const std::string& name, const bool scrollable = true, const bool hasHeader = true);
        static void End();

        static bool isHovered(const PetrichorRendererAPI::Data::Rect& rect);
        
        static void Divider();
        static void Label(const std::string& text, const bool centered = false);
        static void Title(const std::string& text);
        static bool Button(const std::string& text, const PetrichorEngine::Math::Vector2& size, const bool invisible = false);
        static void TextInput(const PetrichorEngine::Math::Vector2& size, std::string& text);
    };
}
