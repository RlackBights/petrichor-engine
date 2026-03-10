#pragma once

#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "freetype/freetype.h"
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <SDL3/SDL_video.h>

namespace RendererBackends::SDL3_OpenGL {

    #define GL_CHECK_ERROR() do { \
        for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) \
            std::cout << "[ERROR] 0x" << std::hex << err << std::dec \
                    << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } while (0)

    using namespace PetrichorRendererAPI;
    using namespace PetrichorRendererAPI::Data;

    class RendererBackend : public IRendererBackend
    {
    private:
        SDL_WindowFlags flags = 0;
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;

        Rect screen = {};

        std::map<std::shared_ptr<Text::Font>, std::unique_ptr<FT_Face>> fontCache = {};
        Text::Font* LoadFont(const std::string& path, int fontSize, int atlasSize) override;
        Text::Character LoadGlyph(char32_t character, Text::Font* font) override;
        Text::Character ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph);
    public:
        ~RendererBackend() = default;

        void SetWindowFlags(SDL_WindowFlags flags);
        void InitializeWindow(const std::string& title, int width, int height) override;
        void InitializeRenderer() override;
        Rect GetScreenRect() override;
        void ResizeWindow(int width, int height) override;

        void InitializeFrame() override;
        void SetCursorIcon(uint32_t cursorID) override;

        void DrawText(const std::string text, Text::Font* font, const Data::Material& material) override;
        void DrawRect(const Rect& rect, const Material& material) override;
        void DrawTriangle(const Triangle& triangle, const Material& material) override;
        void DrawQuad(const Quad& quad, const Material& material) override;
        void DrawMesh(const Mesh& mesh, const Material& material) override;

        void WrapFrame() override;
    };
}