#ifndef PTC_MESH_LOADER_HPP
#define PTC_MESH_LOADER_HPP

#include <vector>
#define MESH_PATH "resources/models/"

#include "ptc_mesh.hpp"
#include <string>
class MeshLoader
{
private:
    static std::vector<Mesh> LoadMeshOBJ(std::string fileContent);
public:
    static std::vector<Mesh> LoadMesh(std::string path);
    static std::vector<Mesh> LoadMesh(const char* path);
};

#endif