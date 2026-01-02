#pragma once

#include "Math/Math.h"
#include <ostream>

namespace PetrichorEngine::Rendering {
    struct Vertex
    {
        Math::Vector3 position = {0, 0, 0};
        Math::Vector2 texCoord = {0, 0};
        Math::Vector3 normal = {0, 0, 0};
        Math::Vector4 color = {1, 1, 1, 1};

        bool operator== (Vertex o)
        {
            return this->position == o.position &&
            this->texCoord == o.texCoord &&
            this->normal == o.normal &&
            this->color == o.color;
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vertex& value)
        {
            stream << "{ X: " << value.position.x << ", Y: " << value.position.y << ", Z: " << value.position.z << " }";
            return stream;
        }
    };
}