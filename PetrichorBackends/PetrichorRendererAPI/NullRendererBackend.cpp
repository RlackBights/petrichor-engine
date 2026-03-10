#include "PetrichorRendererAPI/NullRendererBackend.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include <cstdint>
#include <iostream>

namespace PetrichorRendererAPI
{
    Data::Rect NullRendererBackend::GetScreenRect() { return {}; }
    void NullRendererBackend::ResizeWindow(int width, int height) { if (showDebugMessages) std::cout << "Resized window to [" << width << ". " << height << "]\r\n"; }
    void NullRendererBackend::InitializeWindow(const std::string& title, int width, int height) { std::cout << "Skipping window creation" << "\r\n"; }
    void NullRendererBackend::InitializeRenderer() { std::cout << "Initializing renderless backend..." << (showDebugMessages ? "\n[!] Showing debug messages from draw calls" : "") << "\r\n"; }
    void NullRendererBackend::InitializeFrame() { if (showDebugMessages) std::cout << "Initializing render frame"; }
    void NullRendererBackend::DrawText(const std::string text, Text::Font* font, const Data::Material& material) {};
    void NullRendererBackend::DrawRect(const Data::Rect& rect, const Data::Material& material) { if (showDebugMessages) std::cout << rect; }
    void NullRendererBackend::DrawTriangle(const Data::Triangle& triangle, const Data::Material& material) { if (showDebugMessages) std::cout << triangle; }
    void NullRendererBackend::DrawQuad(const Data::Quad& quad, const Data::Material& material) { if (showDebugMessages) std::cout << quad; }
    void NullRendererBackend::DrawMesh(const Data::Mesh& mesh, const Data::Material& material) { if (showDebugMessages) std::cout << mesh; }
    void NullRendererBackend::WrapFrame() { if (showDebugMessages) std::cout << "Wrapping frame"; }
    void NullRendererBackend::SetCursorIcon(uint32_t cursorID) {}

    Text::Font* NullRendererBackend::LoadFont(const std::string& path, int fontsize, int atlasSize)
    {
        if (showDebugMessages) std::cout << "Return with empty font";
        return nullptr;
    }

    Text::Character NullRendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        if (showDebugMessages) std::cout << "Return with no character";
        return Text::Character{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
}