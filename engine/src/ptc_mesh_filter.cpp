#include "ptc_mesh_filter.hpp"
#include "ptc_mesh_loader.hpp"

MeshFilter::MeshFilter() { this->mesh = MeshLoader::CreateEmptyMesh(); }
MeshFilter::MeshFilter(Mesh mesh)
{
    this->mesh = mesh;
}