#include <ptc_text.h>

void TextManager::initTextManager(Shader inputShader, int screenWidth, int screenHeight)
{
    if (inputShader.ShaderProgramID == -1)
    {
        SDL_Log("ERROR::FREETYPE::TEXT_SHADER_ERROR");
        return;
    }

    textShader = inputShader;

    textShader.setMatrix4x4("model", glm::mat4(1.0f));
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight));
    textShader.setMatrix4x4("view", glm::mat4(1.0f));
    Console::Write("--");

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

    if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face))
    {
        SDL_Log("ERROR::FREETYPE::FAILED_TO_LOAD_FONT");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        SDL_Log("ERROR::FREETYTPE::FAILED_TO_LOAD_GLYPH");
        return;
    }
    Console::Write("--");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
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
        Characters.insert(std::pair<char, Character>(c, character));
    }
    Console::Write("--");


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Console::Write("--");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Console::Write("--");

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
void TextManager::renderText(std::string text, float x, float y, float scale, int screenWidth, int screenHeight, glm::vec3 color, bool isUI)
{
    // activate corresponding render state
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    textShader.use();
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight));
    textShader.setFloat3("textColor", color);
    textShader.setInt("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
}

std::map<char, Character> TextManager::Characters;
FT_Library TextManager::ft;
FT_Face TextManager::face;
unsigned int TextManager::VAO, TextManager::VBO;
Shader TextManager::textShader;