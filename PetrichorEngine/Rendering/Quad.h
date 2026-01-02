#pragma once

#include "Math/Math.h"
#include "Rendering/Vertex.h"
#include <ostream>

namespace PetrichorEngine::Rendering
{
    struct Quad
    {
    public:
        Math::TVector4<Vertex> vertices;

        friend std::ostream& operator<<(std::ostream& stream, const Quad& value)
        {
            stream << "{ V1: " << value.vertices[0] << ", V2: " << value.vertices[1] << ", V3: " << value.vertices[2] << ", V4: " << value.vertices[3] << " }";
            return stream;
        }
    };
}