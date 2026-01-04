#pragma once

namespace PetrichorRendererAPI::Data {
    struct Rect {
        int x = 0, y = 0, width = 0, height = 0;
        Rect() = default;
        Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
    };
}