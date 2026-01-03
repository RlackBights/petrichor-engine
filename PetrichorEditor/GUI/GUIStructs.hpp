#pragma once

#include "Math/Math.h"
#include <memory>
#include <string>
#include <variant>

namespace PetrichorEditor::GUI {
    struct LayoutNode;

    struct Rect {
        int x = 0, y = 0, width = 0, height = 0;
        Rect() = default;
        Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
    };

    struct Panel {
        std::string name;
        Rect rect;
        float scrollOffset;
        float currentHeight;
        PetrichorEngine::Math::Vector4 baseColor;
        bool visible;

        Panel(std::string name, Rect rect, PetrichorEngine::Math::Vector4 baseColor = PetrichorEngine::Math::Vector4(-1.0f), bool visible = true)
            : name(std::move(name)), rect(rect), baseColor(baseColor), visible(visible) {}
    };

    enum SplitDirection { SPLIT_HORIZONTAL, SPLIT_VERTICAL };

    typedef struct Split {
        SplitDirection direction;
        float ratio;
        std::unique_ptr<LayoutNode> childA;
        std::unique_ptr<LayoutNode> childB;

        Split ( 
                SplitDirection dir, 
                float r,
                std::unique_ptr<LayoutNode> a, 
                std::unique_ptr<LayoutNode> b
            ) :
            direction(dir),
            ratio(r),
            childA(std::move(a)),
            childB(std::move(b)) {}
    } Split;

    struct LayoutNode : std::variant<std::unique_ptr<Split>, std::shared_ptr<Panel>> {
        using variant::variant;
    };

    struct TextDrawEntry {
        const std::string text;
        PetrichorEngine::Math::Vector2 position;
        Rect clipRect;
        float scrollOffset;
        TextDrawEntry(const std::string text, const PetrichorEngine::Math::Vector2 position, const Rect clipRect, float scrollOffset) : text(text), position(position), clipRect(clipRect), scrollOffset(scrollOffset) {}
    };

    struct QuadDrawEntry {
        const Rect quad;
        Rect clipRect;
        float z;
        const PetrichorEngine::Math::Vector4 color;
        float* scrollOffset;
        QuadDrawEntry(const Rect quad, Rect clipRect, float z, const PetrichorEngine::Math::Vector4 color, float* scrollOffset) : quad(quad), clipRect(clipRect), z(z), color(color), scrollOffset(scrollOffset) {} 
    };
}
