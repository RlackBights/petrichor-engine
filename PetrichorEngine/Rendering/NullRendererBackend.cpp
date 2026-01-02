#include "Rendering/NullRendererBackend.h"
#include "Core/Log.h"
#include "Rendering/MeshRenderer.h"
#include "Rendering/Quad.h"

namespace PetrichorEngine::Rendering
{
    void NullRendererBackend::InitializeRenderer() { Core::Log::Warn(std::string("Initializing renderless backend...") + (showDebugMessages ? "\n[!] Showing debug messages from draw calls" : "")); }
    void NullRendererBackend::InitializeFrame() { if (showDebugMessages) Core::Log::Info("Initializing render frame"); };
    void NullRendererBackend::DrawTriangle(const Triangle& triangle) { if (showDebugMessages) Core::Log::Info(triangle); }
    void NullRendererBackend::DrawQuad(const Quad& quad) { if (showDebugMessages) Core::Log::Info(quad); }
    void NullRendererBackend::DrawMesh(const MeshRenderer& meshRenderer) { if (showDebugMessages) Core::Log::Info(meshRenderer); }
}