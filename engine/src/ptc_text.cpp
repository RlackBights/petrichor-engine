#include "ptc_console.hpp"
#include "ptc_gui_structs.hpp"
#include "ptc_renderer.hpp"
#include "ptc_transform.hpp"
#include <SDL3/SDL_log.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include "ptc_text.hpp"
#include <string>

Text::Text() {};
Text::Text(std::string _text, float _x, float _y, Font* _font, glm::vec4 _color, Shader _shader) : text(_text), font(_font), color(_color), textShader(_shader), position(_x, _y), offset(0, 0)
{
    offset = glm::vec2(0);
    if (_shader.ShaderProgramID == -1)
    {
        SDL_Log("ERROR::FREETYPE::TEXT_SHADER_ERROR");
        return;
    }

    textShader.setMatrix4x4("model", glm::mat4(1.0f));
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screen.width, 0.0f, (float)Renderer::screen.height));
    textShader.setMatrix4x4("view", glm::mat4(1.0f));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    animationFunction = [](float _) -> float { return 0; };
}
void Text::FixedUpdate()
{
    // activate corresponding render state
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    textShader.use();
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screen.width, 0.0f, (float)Renderer::screen.height));
    textShader.setInt("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);

    int lineNum = 1;
    for (char c : text) if (c == '\n') lineNum++;

    float x = (transform->position.x + offset.x), y = (transform->position.y + offset.y + lineNum * font->fontSize / 2.0f);
    // iterate through all characters
    std::string::const_iterator c, i;
    int index = 0, newlineNum;
    glm::vec4 renderColor = color;
    for (c = text.begin(); c != text.end(); c++)
    {
        switch (*c)
        {
            case '\n':
                newlineNum = 0;
                for (i = c; *i == '\n'; i++) newlineNum++;
                // Move to new line
                x = transform->position.x - getPixelWidthSelf(index + newlineNum) / 2.0f; // Reset x position
                y -= font->fontSize * newlineNum; // Move down by one line
                c += newlineNum - 1;
                continue; // Skip rendering the newline character
            case '[':
                if ((c + 7) < text.end() && *(c + 7) == ']') {
                    bool isCorrect = true;
                    for (int i = 1; i < 7 && isCorrect; i++) {
                        if (*(c + i) < '0' || *(c + i) > 'f' || (*(c + i) > '9' && *(c + i) < 'A') || (*(c + i) > 'F' && *(c + i) < 'a')) isCorrect = false;
                    }
                    if (!isCorrect) break;
                    renderColor.x = std::stoi(std::string(1, (char)*(c + 1)) + (char)*(c + 2), 0, 16) / 255.0f;
                    renderColor.y = std::stoi(std::string(1, (char)*(c + 3)) + (char)*(c + 4), 0, 16) / 255.0f;
                    renderColor.z = std::stoi(std::string(1, (char)*(c + 5)) + (char)*(c + 6), 0, 16) / 255.0f;
                    c += 7;
                    continue;
                } else {
                    break;
                }
            default:
                break;
        }

        textShader.setFloat4("textColor", renderColor);

        Character ch = font->characters[*c];

        float xpos = x + (c == text.begin() ? 0 : ch.Bearing.x) * transform->scale.x;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * transform->scale.y + animationFunction(xpos - transform->position.x);

        float w = ch.Size.x * transform->scale.x;
        float h = ch.Size.y * transform->scale.y;
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
        x += (ch.Advance >> 6) * transform->scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
        index++;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
}
void Text::ForceDrawText(const glm::vec2& position, const Rect& scissor)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScissor(scissor.x, Renderer::screen.height - scissor.y - scissor.height, scissor.width, scissor.height);
    glEnable(GL_SCISSOR_TEST);

    // activate corresponding render state
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    textShader.use();
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screen.width, 0.0f, (float)Renderer::screen.height));
    textShader.setInt("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    //glDisable(GL_DEPTH_TEST);

    int lineNum = 1;
    for (char c : text) if (c == '\n') lineNum++;

    float x = (position.x + offset.x), y = (position.y + offset.y);
    // iterate through all characters
    std::string::const_iterator c, i;
    int index = 0, newlineNum, tabNum;
    glm::vec4 renderColor = color;
    for (c = text.begin(); c != text.end(); c++)
    {
        switch (*c)
        {
            case '\n':
                newlineNum = 0;
                for (i = c; *i == '\n'; i++) newlineNum++;
                // Move to new line
                x = position.x - getPixelWidthSelf(index + newlineNum) / 2.0f; // Reset x position
                y -= font->fontSize * newlineNum; // Move down by one line
                c += newlineNum - 1;
                continue; // Skip rendering the newline character
            case '[':
                if ((c + 7) < text.end() && *(c + 7) == ']') {
                    bool isCorrect = true;
                    for (int i = 1; i < 7 && isCorrect; i++) {
                        if (*(c + i) < '0' || *(c + i) > 'f' || (*(c + i) > '9' && *(c + i) < 'A') || (*(c + i) > 'F' && *(c + i) < 'a')) isCorrect = false;
                    }
                    if (!isCorrect) break;
                    renderColor.x = std::stoi(std::string(1, (char)*(c + 1)) + (char)*(c + 2), 0, 16) / 255.0f;
                    renderColor.y = std::stoi(std::string(1, (char)*(c + 3)) + (char)*(c + 4), 0, 16) / 255.0f;
                    renderColor.z = std::stoi(std::string(1, (char)*(c + 5)) + (char)*(c + 6), 0, 16) / 255.0f;
                    c += 7;
                    continue;
                } else {
                    break;
                }
            case '\t':
                tabNum = 0;
                for (i = c; *i == '\t'; i++) tabNum++;
                x = (((int(x / getPixelWidth(" ")) / 4) + tabNum) * 4) * getPixelWidth(" ");
                c += tabNum;
            default:
                break;
        }

        textShader.setFloat4("textColor", renderColor);

        Character ch = font->characters[*c];

        float xpos = x + (c == text.begin() ? 0 : ch.Bearing.x);
        float ypos = y - (ch.Size.y - ch.Bearing.y);

        float w = ch.Size.x;
        float h = ch.Size.y;
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
        x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
        index++;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
    glDisable(GL_SCISSOR_TEST);
}
void Text::StaticDrawText(const std::string& text, const glm::vec2& position, const Rect& scissor)
{
    if (Text::staticText.text == "") Text::staticText = Text(text, 0, 0, Font::LoadFont("arial.ttf", 100));
    Text::staticText.SetText(text);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glScissor(scissor.x, Renderer::screen.height - scissor.y - scissor.height, scissor.width, scissor.height);
    glEnable(GL_SCISSOR_TEST);

    // activate corresponding render state
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Text::staticText.textShader.use();
    Text::staticText.textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screen.width, 0.0f, (float)Renderer::screen.height));
    Text::staticText.textShader.setInt("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(Text::staticText.VAO);

    //glDisable(GL_DEPTH_TEST);

    int lineNum = 1;
    for (char c : text) if (c == '\n') lineNum++;

    float x = (position.x + Text::staticText.offset.x), y = (position.y + Text::staticText.offset.y);
    // iterate through all characters
    std::string::const_iterator c, i;
    int index = 0, newlineNum;
    glm::vec4 renderColor = glm::vec4(1.0f);
    for (c = text.begin(); c != text.end(); c++)
    {
        switch (*c)
        {
            case '\n':
                newlineNum = 0;
                for (i = c; *i == '\n'; i++) newlineNum++;
                // Move to new line
                x = position.x - Text::staticText.getPixelWidthSelf(index + newlineNum) / 2.0f; // Reset x position
                y -= Text::staticText.font->fontSize * newlineNum; // Move down by one line
                c += newlineNum - 1;
                continue; // Skip rendering the newline character
            case '[':
                if ((c + 7) < text.end() && *(c + 7) == ']') {
                    bool isCorrect = true;
                    for (int i = 1; i < 7 && isCorrect; i++) {
                        if (*(c + i) < '0' || *(c + i) > 'f' || (*(c + i) > '9' && *(c + i) < 'A') || (*(c + i) > 'F' && *(c + i) < 'a')) isCorrect = false;
                    }
                    if (!isCorrect) break;
                    renderColor.x = std::stoi(std::string(1, (char)*(c + 1)) + (char)*(c + 2), 0, 16) / 255.0f;
                    renderColor.y = std::stoi(std::string(1, (char)*(c + 3)) + (char)*(c + 4), 0, 16) / 255.0f;
                    renderColor.z = std::stoi(std::string(1, (char)*(c + 5)) + (char)*(c + 6), 0, 16) / 255.0f;
                    c += 7;
                    continue;
                } else {
                    break;
                }
            default:
                break;
        }

        Text::staticText.textShader.setFloat4("textColor", glm::vec4(1.0f));

        Character ch = Text::staticText.font->characters[*c];

        float xpos = x + (c == text.begin() ? 0 : ch.Bearing.x);
        float ypos = y - (ch.Size.y - ch.Bearing.y);

        float w = ch.Size.x;
        float h = ch.Size.y;
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
        glBindBuffer(GL_ARRAY_BUFFER, Text::staticText.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
        index++;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
    glDisable(GL_SCISSOR_TEST);
}
int Text::getStaticPixelWidth(const std::string &text, const int fontSize)
{
    if (Text::staticText.text == "") Text::staticText = Text(text, 0, 0, Font::LoadFont("arial.ttf", 100));
    Text::staticText.SetText(text);
    int width = 0;

    std::string::const_iterator c;
    int index = 0;
    for (c = text.begin(); c != text.end(); c++)
    {
        if ((c + 7) < text.end() && *(c + 7) == ']') {
            bool isCorrect = true;
            for (int i = 1; i < 7 && isCorrect; i++) {
                if (*(c + i) < '0' || *(c + i) > 'f' || (*(c + i) > '9' && *(c + i) < 'A') || (*(c + i) > 'F' && *(c + i) < 'a')) isCorrect = false;
            }
            if (!isCorrect) break;
            c += 7;
            continue;
        }
        if (0 > index++) continue;
        if (Text::staticText.font->characters.find(*c) != Text::staticText.font->characters.end()) {
            Character ch = Text::staticText.font->characters[*c];
            width += (int)((ch.Advance >> 6) * ((Text::staticText.transform != nullptr) ? Text::staticText.transform->scale.x : 1));
        }
    }

    return width * fontSize / Text::staticText.font->fontSize;
}
void Text::Awake()
{
    if (parentObject == nullptr) return;
    transform->position.x = position.x;
    transform->position.y = position.y;
}
void Text::SetTextAnimation(std::function<float(float _x)> _animationFunction)
{
    this->animationFunction = _animationFunction;
}
void Text::MoveText(int _x, int _y, bool _centered)
{
    transform->position.x = _x;
    transform->position.y = _y;
    if (_centered) CenterText();
}
void Text::CenterText()
{
    offset.x = -getPixelWidthSelf() / 2.0f;
    offset.y = -font->fontSize / 2.0f;
}
void Text::SetText(std::string _text, bool _updatePosition)
{
    text = _text;
    if (_updatePosition) CenterText();
}
std::string Text::GetText()
{
    return text;
}
void Text::SetTextColor(glm::vec4 _color)
{
    color = _color;
}
glm::vec4 Text::GetTextColor()
{
    return color;
}
int Text::getPixelWidthSelf(int _index, bool _ignoreLinebreak)
{
    return Text::getPixelWidth(this->text);
}
int Text::getPixelWidth(const std::string& text, int _index, bool _ignoreLinebreak)
{
    int width = 0;

    std::string::const_iterator c;
    int index = 0;
    for (c = text.begin(); c != text.end(); c++)
    {
        if ((c + 7) < text.end() && *(c + 7) == ']') {
            bool isCorrect = true;
            for (int i = 1; i < 7 && isCorrect; i++) {
                if (*(c + i) < '0' || *(c + i) > 'f' || (*(c + i) > '9' && *(c + i) < 'A') || (*(c + i) > 'F' && *(c + i) < 'a')) isCorrect = false;
            }
            if (!isCorrect) break;
            c += 7;
            continue;
        }
        if (_index > index++) continue;
        if (font->characters.find(*c) != font->characters.end()) {
            if (!_ignoreLinebreak && *c == '\n') return width;
            Character ch = font->characters[*c];
            width += (int)((ch.Advance >> 6) * ((transform != nullptr) ? transform->scale.x : 1));
        }
    }

    return width;
}
Font* Text::GetFont()
{
    return font;
}

Text Text::staticText;