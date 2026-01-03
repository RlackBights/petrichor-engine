#include "Rendering/MeshRenderer.h"
#include "Rendering/MeshFilter.h"

namespace PetrichorEngine::Rendering {
    MeshRenderer::MeshRenderer()
    {
        meshFilter = parentObject->GetComponent<MeshFilter>();
        if (!meshFilter) parentObject->RemoveComponent<MeshRenderer>();
    }
}