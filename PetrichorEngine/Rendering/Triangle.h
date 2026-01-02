#pragma once

#include "Math/Math.h"
#include "Rendering/Vertex.h"
namespace PetrichorEngine::Rendering
{
    class Triangle
    {
    public:
        Math::TVector3<Vertex> vertices;

        friend std::ostream& operator<<(std::ostream& stream, const Triangle& value)
        {
            stream << "{ V1: " << value.vertices[0] << ", V2: " << value.vertices[1] << ", V3: " << value.vertices[2] << " }";
            return stream;
        }
    };
}