#pragma once

#include "Math/Math.h"
#include <memory>
#include <string>
#include <variant>
#include "PetrichorRendererAPI/Data/Rect.h"

namespace PetrichorEditor {
    struct LayoutNode;

    struct Panel {
        std::string name;
        PetrichorRendererAPI::Data::Rect rect;
        float scrollOffset;
        float currentHeight;
        PetrichorEngine::Math::Vector4 baseColor;
        bool visible;

        Panel(std::string name, PetrichorRendererAPI::Data::Rect rect, PetrichorEngine::Math::Vector4 baseColor = PetrichorEngine::Math::Vector4(-1.0f), bool visible = true)
            : name(std::move(name)), rect(rect), baseColor(baseColor), visible(visible) {}
    };

    enum class SplitDirection { SPLIT_HORIZONTAL, SPLIT_VERTICAL };

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
        PetrichorRendererAPI::Data::Rect clipRect;
        float scrollOffset;
        TextDrawEntry(const std::string text, const PetrichorEngine::Math::Vector2 position, const PetrichorRendererAPI::Data::Rect clipRect, float scrollOffset) : text(text), position(position), clipRect(clipRect), scrollOffset(scrollOffset) {}
    };

    struct RectDrawEntry {
        const PetrichorRendererAPI::Data::Rect rect;
        int z;
        PetrichorRendererAPI::Data::Rect clipRect;
        const PetrichorEngine::Math::Vector4 color;
        float* scrollOffset;
        RectDrawEntry(const PetrichorRendererAPI::Data::Rect rect, int z, PetrichorRendererAPI::Data::Rect clipRect, const PetrichorEngine::Math::Vector4 color, float* scrollOffset) : rect(rect), clipRect(clipRect), z(z), color(color), scrollOffset(scrollOffset) {} 
    };
}
