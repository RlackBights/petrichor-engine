#include "IO/OBJMeshLoader.h"
#include "Core/Log.h"
#include "IO/FileProcessor.h"
#include "IO/MeshHierarchy.h"
#include "Rendering/Mesh.h"
#include "Rendering/Vertex.h"
#include <filesystem>
#include <locale>
#include <memory>
#include <variant>
#include <map>
#include <sstream>

namespace PetrichorEngine::IO {
    OBJFaceToken::OBJFaceToken(const std::string& token) {
        if (token.find('/') == std::variant_npos) {
            sscanf(token.c_str(), "%d", &this->v);
        } else if (token.substr(token.find('/') + 1).find('/') == std::variant_npos) {
            sscanf(token.c_str(), "%d/%d", &this->v, &this->vt);
        } else if (token.find("//") != std::variant_npos) {
            sscanf(token.c_str(), "%d//%d", &this->v, &this->vn);
        } else {
            sscanf(token.c_str(), "%d/%d/%d", &this->v, &this->vt, &this->vn);
        }
    }

    MeshHierarchy OBJMeshLoader::GenerateMeshHierarchy(const std::string& path)
    {
        std::string fileContent = FileProcessor::Read(path);

        std::string line = "";
        std::map<std::string, Rendering::Mesh> meshes;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;

        float x, y, z;
        std::istringstream stream;
        std::string token;
        std::vector<int> indices;

        std::vector<OBJFaceToken> processedTokens;
        
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
                                Core::Log::Error(std::string("Error parsing .obj character: ") + line[0] + line[1]);
                                break;
                        }
                        break;
                    case 'f':
                        stream = std::istringstream(line.substr(2));
                        indices = {};
                        while (stream >> token) {
                            OBJFaceToken ft = OBJFaceToken(token);
                            Rendering::Vertex vert = Rendering::Vertex(vertices[ft.v - 1], texCoords[ft.vt - 1], normals[ft.vn - 1]);
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
                        meshes.insert({line.substr(2), Rendering::Mesh{line.substr(2), {}, {}, 0}});
                        break;
                    case 'l':   // polylines? really?
                        break;
                    case 'm':   // material-related stuff maybe
                        break;
                    case 'u':   // use material
                        break;
                    case '#':   // Comment, ignore
                        break;
                    case '\0':  // empty line
                        break;
                    default:
                        Core::Log::Error(std::string("Error parsing .obj character: ") + line[0]);
                        break;
                }
                line = "";
            } else {
                line += c;
            }
        }

        MeshHierarchy hierarchy{ nullptr, std::make_unique<Rendering::Mesh>(), {}};
        hierarchy.mesh->name = std::filesystem::path(path).stem();

        for (auto& [name, mesh] : meshes) {
            hierarchy.children.push_back(
                {
                    &hierarchy,
                    std::make_unique<Rendering::Mesh>(mesh),
                    {}
                }
            );
        }

        return hierarchy;
    }
}