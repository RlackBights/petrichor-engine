#include "ptc_console.hpp"
#include "ptc_layout_structs.hpp"
#include "ptc_renderer.hpp"
#include "ptc_shader.hpp"
#include <cstdio>
#include <glm/fwd.hpp>
#include <memory>
#include <ptc_gui.hpp>
#include <vector>

void GUI::InitUI()
{
    cursorPos = {0, 0};
    shader = Shader("gui_vert.glsl", "gui_frag.glsl");
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
    
    GUI::layout = LayoutNode (
        std::make_unique<Split>(
            SplitDirection::SPLIT_VERTICAL,
            0.77f,
            std::make_unique<LayoutNode>(
                std::make_unique<Split>(
                    SplitDirection::SPLIT_HORIZONTAL,
                    0.33,
                    std::make_unique<LayoutNode>(
                        std::make_shared<Panel>("Console", Rect(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
                    ),
                    std::make_unique<LayoutNode>(
                        std::make_unique<Split>(
                            SplitDirection::SPLIT_VERTICAL,
                            0.27f,
                            std::make_unique<LayoutNode>(
                                std::make_shared<Panel>("Hierarchy", Rect(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
                            ),
                            std::make_unique<LayoutNode>(
                                std::make_shared<Panel>("Scene", Rect())
                            )
                        )
                    )
                )
            ),
            std::make_unique<LayoutNode>(
                std::make_shared<Panel>("Inspector", Rect(), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
            )
        )
    );
}
void GUI::ApplyLayout()
{
    GUI::CalculateRects(layout, Renderer::screen);
}
void GUI::CalculateRects(LayoutNode& node, Rect area) {
    if (std::holds_alternative<std::shared_ptr<Panel>>(node)) {
        auto panel = std::get<std::shared_ptr<Panel>>(node);
        panel->rect = area;
        if (panel->name == "Scene") 
        {
            Renderer::viewport = panel->rect;
        } else {
            Console::WriteLine(panel->name);
            GUI::DrawQuad(panel->rect.x, Renderer::screen.height - panel->rect.y - panel->rect.height, 0, panel->rect.width, panel->rect.height, panel->baseColor);
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
    } else {
        int widthA = static_cast<int>(area.width * split.ratio);
        int widthB = area.width - widthA;

        rectA = Rect(area.x, area.y, widthA, area.height);
        rectB = Rect(area.x + widthA, area.y, widthB, area.height);
    }

    CalculateRects(*a, rectA);
    CalculateRects(*b, rectB);
}

void GUI::DrawQuad(float _x, float _y, float _z, float _w, float _h, glm::vec4 _color)
{
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);

    batchedVertices.push_back(_x);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y + _h);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
    
    batchedVertices.push_back(_x + _w);
    batchedVertices.push_back(_y);
    batchedVertices.push_back(_z);
    batchedVertices.push_back(_color.r);
    batchedVertices.push_back(_color.g);
    batchedVertices.push_back(_color.b);
    batchedVertices.push_back(_color.a);
}
void GUI::RenderUI()
{
    if (batchedVertices.empty()) return;
    shader.use();
    shader.setFloat2("screenSize", { Renderer::screen.width, Renderer::screen.height });
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // for (size_t i = 0; i < batchedVertices.size(); i += 7) {
    //     printf("%f %f %f %f %f %f %f\n",
    //         batchedVertices[i + 0],  // x
    //         batchedVertices[i + 1],  // y
    //         batchedVertices[i + 2],  // z
    //         batchedVertices[i + 3],  // r
    //         batchedVertices[i + 4],  // g
    //         batchedVertices[i + 5],  // b
    //         batchedVertices[i + 6]   // a
    //     );
    // }
    // printf("\n");
    //batchedVertices.clear();

    float vertices[] = {
        // positions (pixel coords)          // colors
        0.f,      0, 0.f, 1, 1, 1, 1,  // top-left
        0, (float)Renderer::screen.height, 0.f, 1, 1, 1, 1,  // top-right
        
        (float)Renderer::screen.width, (float)Renderer::screen.height, 0.f, 1, 1, 1, 1 ,  // bottom-center
        0.f,      0, 0.f, 1, 1, 1, 1,  // top-left
        (float)Renderer::screen.width, (float)Renderer::screen.height, 0.f, 1, 1, 1, 1 ,  // bottom-center
        (float)Renderer::screen.width,      0, 0.f, 1, 1, 1, 1,  // top-left
    };

    //DrawQuad(0, 0, 0, (float)Renderer::screen.width, (float)Renderer::screen.height, glm::vec4(1.0f));
    for (size_t i = 0; i < batchedVertices.size(); i += 7) {
        printf("%f %f %f %f %f %f %f\n",
            batchedVertices[i + 0],  // x
            batchedVertices[i + 1],  // y
            batchedVertices[i + 2],  // z
            batchedVertices[i + 3],  // r
            batchedVertices[i + 4],  // g
            batchedVertices[i + 5],  // b
            batchedVertices[i + 6]   // a
        );
    }
    printf("\n");
    glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), batchedVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, batchedVertices.size() / 7);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    batchedVertices.clear();
}

std::vector<float> GUI::batchedVertices;
Shader GUI::shader;
GLuint GUI::VBO, GUI::VAO;
glm::vec2 GUI::cursorPos;
LayoutNode GUI::layout;