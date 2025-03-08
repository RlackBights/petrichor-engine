#include <ptc_text.h>

Text::Text(std::string _text, float x, float y, Font* _font, glm::vec4 _color, Shader _shader) : text(_text), font(_font), color(_color), textShader(_shader), position(x, y)
{
    if (_shader.ShaderProgramID == -1)
    {
        SDL_Log("ERROR::FREETYPE::TEXT_SHADER_ERROR");
        return;
    }

    textShader.setMatrix4x4("model", glm::mat4(1.0f));
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screenWidth, 0.0f, (float)Renderer::screenHeight));
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
}
void Text::FixedUpdate()
{
    // activate corresponding render state
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    textShader.use();
    textShader.setMatrix4x4("projection", glm::ortho(0.0f, (float)Renderer::screenWidth, 0.0f, (float)Renderer::screenHeight));
    textShader.setFloat4("textColor", color);
    textShader.setInt("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);

    float x = position.x, y = position.y;
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = font->characters[*c];

        float xpos = x + ch.Bearing.x * parentObject->transform.scale.x;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * parentObject->transform.scale.y;

        float w = ch.Size.x * parentObject->transform.scale.x;
        float h = ch.Size.y * parentObject->transform.scale.y;
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
        x += (ch.Advance >> 6) * parentObject->transform.scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
}