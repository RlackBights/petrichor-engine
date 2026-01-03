#pragma once

#include "Vertex.h"

namespace PetrichorRendererAPI
{
    class Triangle
    {
    public:
        Vertex vertices[3];

        friend std::ostream& operator<<(std::ostream& stream, const Triangle& value)
        {
            stream << "{ V1: " << value.vertices[0] << ", V2: " << value.vertices[1] << ", V3: " << value.vertices[2] << " }";
            return stream;
        }
    };
}