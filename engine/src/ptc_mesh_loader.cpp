#include "ptc_mesh_loader.hpp"
#include "glm/fwd.hpp"
#include "ptc_console.hpp"
#include "ptc_face_token.hpp"
#include "ptc_file_reader.hpp"
#include "ptc_mesh.hpp"
#include "ptc_vertex.hpp"
#include <filesystem>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, Mesh> MeshLoader::LoadMeshOBJ(std::string fileContent)
{
    std::string line = "";
    std::map<std::string, Mesh> meshes;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    float x, y, z;
    std::istringstream stream;
    std::string token;
    std::vector<int> indices;

    std::vector<FaceToken> processedTokens;
    
    for (char c : fileContent) {
        if (c == '\n') {
            switch (line[0]) {
                case 'v':
                    switch (line[1]) {
                        case ' ':
                            sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                            vertices.push_back(glm::vec3(x, y, z));
                            break;
                        case 'n':
                            sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                            normals.push_back(glm::vec3(x, y, z));
                            break;
                        case 't':
                            sscanf(line.c_str(), "vt %f %f", &x, &y);
                            texCoords.push_back(glm::vec2(x, y));
                            break;
                        default:
                            Console::WriteLine(Console::FormatString("Error parsing .obj character: \'%c%c\'", line[0], line[1]));
                            break;
                    }
                    break;
                case 'f':
                    stream = std::istringstream(line.substr(2));
                    indices = {};
                    while (stream >> token) {
                        FaceToken ft = FaceToken(token);
                        Vertex vert = Vertex(vertices[ft.v - 1], texCoords[ft.vt - 1], normals[ft.vn - 1]);
                        meshes.rbegin()->second.vertices.push_back(vert);
                        indices.push_back(meshes.rbegin()->second.vertices.size() - 1);
                    }

                    for (int i = 1; i < indices.size() - 1; i++) {
                        meshes.rbegin()->second.indices.push_back(indices[0]);
                        meshes.rbegin()->second.indices.push_back(indices[i]);
                        meshes.rbegin()->second.indices.push_back(indices[i + 1]);
                    }

                    break;
                case 's':   // smoothing 
                    if (line[2] == 'o') {
                        meshes.rbegin()->second.smoothing = 0;
                    } else {
                        meshes.rbegin()->second.smoothing = line[2] - '0';
                    }
                    break;
                case 'o':   // object
                    meshes.insert({line.substr(2), Mesh(line.substr(2))});
                    break;
                case 'm':   // material-related stuff maybe
                    break;
                case 'u':   // use material
                    break;
                case '#':   // Comment, ignore
                    break;
                case '\0': // empty line
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

    for (auto& [name, mesh] : meshes) {
        mesh.RegisterBuffers();
    }

    return meshes;
}

std::map<std::string, Mesh> MeshLoader::LoadMesh(std::string path)
{
    std::string ext = std::filesystem::path(MESH_PATH + path).extension().string();
    if (ext == ".obj") {
        return MeshLoader::LoadMeshOBJ(FileReader::Read(MESH_PATH + path));
    } else {
        return {{ "", MeshLoader::CreateEmptyMesh() } };
    }
    
}

std::map<std::string, Mesh> MeshLoader::LoadMesh(const char* path)
{
    return MeshLoader::LoadMesh(std::string(path));
}

Mesh MeshLoader::CreateEmptyMesh()
{
    return Mesh();
}