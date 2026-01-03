#pragma once

#include "Vertex.h"
#include <ostream>

namespace PetrichorRendererAPI
{
    struct Quad
    {
    public:
        Vertex vertices[4];

        friend std::ostream& operator<<(std::ostream& stream, const Quad& value)
        {
            stream << "{ V1: " << value.vertices[0] << ", V2: " << value.vertices[1] << ", V3: " << value.vertices[2] << ", V4: " << value.vertices[3] << " }";
            return stream;
        }
    };
}