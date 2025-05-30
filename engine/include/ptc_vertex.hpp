#ifndef PTC_VERTEX_HPP
#define PTC_VERTEX_HPP

#include <glm/glm.hpp>
#include "glm/fwd.hpp"

struct Vertex
{
    glm::vec3 position = {0, 0, 0};
    glm::vec3 normal = {0, 0, 0};
    glm::vec3 texCoord = {0, 0, 0};
    glm::vec3 tangent = {0, 0, 0};
    glm::vec3 bitangent = {0, 0, 0};
    glm::vec4 color = {0, 0, 0, 0};
};

#endif