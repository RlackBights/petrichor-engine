#include "IO/OBJMeshLoader.h"
#include "Core/Log.h"
#include "IO/FileProcessor.h"
#include "IO/MeshHierarchy.h"
#include "Math/Math.h"
#include "PetrichorRendererAPI/Mesh.h"
#include "PetrichorRendererAPI/Vertex.h"
#include <filesystem>
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
        std::map<std::string, PetrichorRendererAPI::Mesh> meshes;

        std::vector<Math::Vector3> vertices;
        std::vector<Math::Vector3> normals;
        std::vector<Math::Vector2> texCoords;

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
                                vertices.push_back(Math::Vector3(x, y, z));
                                break;
                            case 'n':
                                sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                                normals.push_back(Math::Vector3(x, y, z));
                                break;
                            case 't':
                                sscanf(line.c_str(), "vt %f %f", &x, &y);
                                texCoords.push_back(Math::Vector2(x, y));
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

                            float vertPos[3] = {vertices[ft.v - 1].x, vertices[ft.v - 1].y, vertices[ft.v - 1].z};
                            float vertTex[2] = {texCoords[ft.vt - 1].x, texCoords[ft.vt - 1].y};
                            float vertNorm[3] = {normals[ft.vn - 1].x, normals[ft.vn - 1].y, normals[ft.vn - 1].z};

                            PetrichorRendererAPI::Vertex vert = PetrichorRendererAPI::Vertex{vertPos, vertTex, vertNorm, nullptr};
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
                        meshes.insert({line.substr(2), PetrichorRendererAPI::Mesh{line.substr(2), {}, {}, 0}});
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

        MeshHierarchy hierarchy{ nullptr, std::make_unique<PetrichorRendererAPI::Mesh>(), {}};
        hierarchy.mesh->name = std::filesystem::path(path).stem();

        for (auto& [name, mesh] : meshes) {
            hierarchy.children.push_back(
                {
                    &hierarchy,
                    std::make_unique<PetrichorRendererAPI::Mesh>(mesh),
                    {}
                }
            );
        }

        return hierarchy;
    }
}