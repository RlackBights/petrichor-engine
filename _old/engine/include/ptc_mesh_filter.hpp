#ifndef PTC_MESH_FILTER_HPP
#define PTC_MESH_FILTER_HPP

#include "ptc_component.hpp"
#include "ptc_mesh.hpp"
#include "ptc_reflection.hpp"

class MeshFilter : public Component
{
friend class Reflection;
private:
    
public:
    Mesh mesh;

    MeshFilter();
    MeshFilter(Mesh mesh);
};

#endif