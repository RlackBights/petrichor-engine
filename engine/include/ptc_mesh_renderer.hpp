#ifndef PTC_MESH_RENDERER_HPP
#define PTC_MESH_RENDERER_HPP

#include "ptc_component.hpp"
#include "ptc_material.hpp"
#include "ptc_mesh_filter.hpp"

class MeshRenderer : public Component
{
private:
    MeshFilter* filterRef;

    bool FindMeshFilter();
public:
    Material material;

    MeshRenderer();
    MeshRenderer(Material material);

    void Start() override;
    void Update() override;
};

#endif