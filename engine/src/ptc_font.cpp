#include <freetype/fttypes.h>
#include <ptc_font.hpp>

Font::Font(std::string _path, int _fontSize) : path(_path), fontSize(_fontSize)
{
    if (SDL_GL_GetCurrentContext() == NULL)
    {
        SDL_Log("ERROR::FREETYPE::FAILED_TO_GET_OPENGL_CONTEXT");
        return;
    }

    if (FT_Init_FreeType(&ft))
    {
        SDL_Log("ERROR::FREETYPE::COULD_NOT_INITIALIZE_LIB");
        return;
    }

    if (FT_New_Face(ft, std::string(FONT_PATH + _path).c_str(), 0, &face))
    {
        SDL_Log("ERROR::FREETYPE::FAILED_TO_LOAD_FONT\nDefaulting to arial...");
        path = "arial.ttf";
        if (FT_New_Face(ft, (std::string(FONT_PATH) + "arial.ttf").c_str(), 0, &face))
        {
            SDL_Log("ERROR::FREETYPE::FAILED_TO_LOAD_FONT\nFailed to load arial.ttf");
            return;
        }
    }

    FT_Set_Pixel_Sizes(face, 0, _fontSize);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        SDL_Log("ERROR::FREETYTPE::FAILED_TO_LOAD_GLYPH");
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (FT_ULong c = 0; c < 1024; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            SDL_Log("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
}
Font* Font::LoadFont(std::string _path, int _fontSize)
{
    for (const auto& [cachePath, cacheFont] : fontCache) {
        if (cachePath == _path && fontCache[_path]->fontSize == _fontSize) {
            return fontCache[_path];
        }
    }
    return new Font(_path, _fontSize);
}

std::map<std::string, Font*> Font::fontCache;
FT_Library Font::ft;