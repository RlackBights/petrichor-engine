#include "NullRendererBackend.h"
#include "Text/Font.h"
#include <iostream>

namespace PetrichorRendererAPI
{
    void NullRendererBackend::InitializeRenderer() { std::cout << "Initializing renderless backend..." << (showDebugMessages ? "\n[!] Showing debug messages from draw calls" : "") << "\r\n"; }
    void NullRendererBackend::InitializeFrame() { if (showDebugMessages) std::cout << "Initializing render frame"; };
    void NullRendererBackend::DrawTriangle(const Triangle& triangle) { if (showDebugMessages) std::cout << triangle; }
    void NullRendererBackend::DrawQuad(const Quad& quad) { if (showDebugMessages) std::cout << quad; }
    void NullRendererBackend::DrawMesh(const Mesh& mesh) { if (showDebugMessages) std::cout << mesh; }

    Text::Font NullRendererBackend::LoadFont(const std::string& path, int fontsize, int atlasSize)
    {
        if (showDebugMessages) std::cout << "Return with empty font";
        return {"null", -1, {}, {}};
    }

    Text::Character NullRendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        if (showDebugMessages) std::cout << "Return with no character";
        return Text::Character{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
}