#ifndef PTC_VERTEX_HPP
#define PTC_VERTEX_HPP

#include <glm/glm.hpp>
#include "glm/fwd.hpp"

struct Vertex
{
    glm::vec3 position = {0, 0, 0};
    glm::vec3 normal = {0, 0, 0};
    glm::vec2 texCoord = {0, 0};
    glm::vec3 tangent = {0, 0, 0};
    glm::vec3 bitangent = {0, 0, 0};
    glm::vec4 color = {0, 0, 0, 0};

    bool operator== (Vertex o)
    {
        return this->position == o.position &&
        this->texCoord == o.texCoord &&
        this->normal == o.normal &&
        this->tangent == o.tangent &&
        this->bitangent == o.bitangent &&
        this->color == o.color;
    }

    Vertex( glm::vec3 position = {0, 0, 0}, 
            glm::vec2 texCoord = {0, 0},
            glm::vec3 normal = {0, 0, 0},
            glm::vec3 tangent = {0, 0, 0},
            glm::vec3 bitangent = {0, 0, 0},
            glm::vec4 color = {0, 0, 0, 0} )
    {
        this->position = position;
        this->texCoord = texCoord;
        this->normal = normal;
        this->tangent = tangent;
        this->bitangent = bitangent;
        this->color = color;
    }
};

#endif