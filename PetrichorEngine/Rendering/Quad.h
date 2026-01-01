#pragma once

#include "Math/Math.h"
#include "Rendering/Vertex.h"
namespace PetrichorEngine::Rendering
{
    struct Quad
    {
    public:
        Math::TVector4<Vertex> vertices;
    };
}