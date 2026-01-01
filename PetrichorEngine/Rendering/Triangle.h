#pragma once

#include "Math/Math.h"
#include "Rendering/Vertex.h"
namespace PetrichorEngine::Rendering
{
    class Triangle
    {
    public:
        Math::TVector3<Vertex> vertices;
    };
}