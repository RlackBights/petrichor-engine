#include "Rendering/MeshRenderer.h"
#include "Rendering/MeshFilter.h"
namespace PetrichorEngine::Rendering {
    MeshRenderer::MeshRenderer()
    {
        meshFilter = nullptr;
    }

    void MeshRenderer::Start()
    {
        meshFilter = parentObject->GetComponent<MeshFilter>();
    }
}