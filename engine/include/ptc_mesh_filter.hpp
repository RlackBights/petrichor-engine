#ifndef PTC_MESH_FILTER_HPP
#define PTC_MESH_FILTER_HPP

#include "ptc_component.hpp"
#include "ptc_mesh.hpp"

class MeshFilter : public Component
{
private:
    
public:
    Mesh mesh;

    MeshFilter();
    MeshFilter(Mesh mesh);
};

#endif