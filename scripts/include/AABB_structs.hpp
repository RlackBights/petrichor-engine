#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include "glm/vec3.hpp"
struct BoundingBox
{
    glm::vec3 upperBound;
    glm::vec3 lowerBound;
};

struct BVHNode
{
    BVHNode* parent;
    BVHNode* left;
    BVHNode* right;
    BoundingBox box;
    bool isLeaf;
};



#endif