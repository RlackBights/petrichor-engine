#pragma once

#include <ostream>
namespace PetrichorRendererAPI::Data {
    struct Rect {
        int x = 0, y = 0, width = 0, height = 0;
        Rect() = default;
        Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}

        friend std::ostream& operator<<(std::ostream& stream, const Rect& value)
        {
            stream << "{ X: " << value.x << ", Y: " << value.y << ", W: " << value.width << ", H: " << value.height << " }";
            return stream;
        }
    };
}