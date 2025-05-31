#ifndef PTC_MESH_LOADER_HPP
#define PTC_MESH_LOADER_HPP

#include <map>
#define MESH_PATH "resources/models/"

#include "ptc_mesh.hpp"
#include <string>
class MeshLoader
{
private:
    static std::map<std::string, Mesh> LoadMeshOBJ(std::string fileContent);
public:
    static std::map<std::string, Mesh> LoadMesh(std::string path);
    static std::map<std::string, Mesh> LoadMesh(const char* path);
    static Mesh CreateEmptyMesh();
};

#endif