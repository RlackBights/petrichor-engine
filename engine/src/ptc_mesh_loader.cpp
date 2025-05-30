#include "ptc_mesh_loader.hpp"
#include "glm/fwd.hpp"
#include "ptc_console.hpp"
#include "ptc_file_reader.hpp"
#include "ptc_mesh.hpp"
#include <filesystem>
#include <string>
#include <vector>

std::vector<Mesh> MeshLoader::LoadMeshOBJ(std::string fileContent)
{
    std::string line = "";
    std::vector<Mesh> meshes;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    for (char c : fileContent) {
        if (c == '\n') {
            switch (line[0]) {
                case 'v':
                    switch (line[1]) {
                        case ' ':
                            break;
                        case 'n':
                            break;
                        case 't':
                            break;
                        default:
                            Console::WriteLine(Console::FormatString("Error parsing .obj character: \'%c%c\'", line[0], line[1]));
                            break;
                    }
                    break;
                case 'f':
                    break;
                case 's':   // smoothing
                    break;
                case 'o':   // object
                    char* name;
                    sscanf(line.c_str(), "o %s", name);
                    meshes.push_back(Mesh(name));
                    break;
                case 'm':   // material-related stuff maybe
                    break;
                case 'u':   // use material
                    break;
                case '#':   // Comment, ignore
                    break;
                default:
                    Console::WriteLine(Console::FormatString("Error parsing .obj character: \'%c\'", line[0]));
                    break;
            }
            line = "";
        } else {
            line += c;
        }
    }

    return meshes;
}

std::vector<Mesh> MeshLoader::LoadMesh(std::string path)
{
    std::string ext = std::filesystem::path(MESH_PATH + path).extension().string();
    if (ext == ".obj") {
        return MeshLoader::LoadMeshOBJ(FileReader::Read(MESH_PATH + path));
    } else {
        return { Mesh() };
    }
    
}

std::vector<Mesh> MeshLoader::LoadMesh(const char* path)
{
    return MeshLoader::LoadMesh(std::string(path));
}