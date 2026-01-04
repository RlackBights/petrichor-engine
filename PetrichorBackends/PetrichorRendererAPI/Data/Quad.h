#pragma once

#include "Vertex.h"
#include <ostream>

namespace PetrichorRendererAPI::Data
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

        Quad(const Vertex& v1 = Vertex(), const Vertex& v2 = Vertex(), const Vertex& v3 = Vertex(), const Vertex& v4 = Vertex())
        {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
            vertices[3] = v4;
        }

        Quad(const Vertex& tl, const Vertex& br)
        {
            vertices[0] = tl;

            Vertex tr;
            tr.position[0] = br.position[0];
            tr.position[1] = tl.position[1];
            tr.position[2] = br.position[2];
            vertices[1] = tr;

            Vertex bl;
            bl.position[0] = tl.position[0];
            bl.position[1] = br.position[1];
            bl.position[2] = tl.position[2];
            vertices[2] = bl;

            vertices[3] = br;
        }
    };
}