#pragma once

#include <algorithm>
#include <ostream>

namespace PetrichorRendererAPI {
    struct Vertex
    {
        float position[3];
        float texCoord[2];
        float normal[3];
        float color[4];

        bool operator== (Vertex o)
        {
            return this->position == o.position &&
            this->texCoord == o.texCoord &&
            this->normal == o.normal &&
            this->color == o.color;
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vertex& value)
        {
            stream << "{ X: " << value.position[0] << ", Y: " << value.position[1] << ", Z: " << value.position[2] << " }";
            return stream;
        }

        Vertex(float position[3], float texCoord[2], float normal[3], float color[4])
        {
            if (!position) std::fill_n(position, 3, 0);
            if (!texCoord) std::fill_n(texCoord, 2, 0);
            if (!normal) std::fill_n(normal, 3, 0);
            if (!color) std::fill_n(color, 4, 1);
            
            for (int i = 0; i < 3; i++) this->position[i] = position[i];
            for (int i = 0; i < 2; i++) this->texCoord[i] = texCoord[i];
            for (int i = 0; i < 3; i++) this->normal[i] = normal[i];
            for (int i = 0; i < 4; i++) this->color[i] = color[i];
        }
    };
}