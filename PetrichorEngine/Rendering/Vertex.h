#pragma once

#include "Math/Math.h"

namespace PetrichorEngine::Rendering {
    struct Vertex
    {
        Math::Vector3 position = {0, 0, 0};
        Math::Vector2 texCoord = {0, 0};
        Math::Vector3 normal = {0, 0, 0};

        bool operator== (Vertex o)
        {
            return this->position == o.position &&
            this->texCoord == o.texCoord &&
            this->normal == o.normal;
        }

        Vertex( const Math::Vector3& position = {0, 0, 0}, 
                const Math::Vector2& texCoord = {0, 0},
                const Math::Vector3& normal = {0, 0, 0} )
        {
            this->position = position;
            this->texCoord = texCoord;
            this->normal = normal;
        }
    };
}