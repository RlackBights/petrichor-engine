#pragma once

#include "ECS/Component.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "Rendering/MeshFilter.h"
#include <ostream>

namespace PetrichorEngine::Rendering {
    class MeshRenderer : public ECS::Component
    {
    private:
        MeshFilter* meshFilter;
    public:
        std::shared_ptr<PetrichorRendererAPI::Data::Material> material;

        MeshRenderer();

        friend std::ostream& operator<<(std::ostream& stream, const MeshRenderer& value)
        {
            stream << "[MeshRenderer]";
            return stream;
        }
    };
}

