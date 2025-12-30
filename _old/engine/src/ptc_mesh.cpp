// #include "ptc_file_reader.hpp"
// #include "ptc_light.hpp"
// #include "ptc_material.hpp"
// #include <glm/ext/matrix_transform.hpp>
// #include <glm/fwd.hpp>
// #include <glm/geometric.hpp>
// #include <ptc_mesh.hpp>
// #include <vector>
// #include <fstream>
// #include <sstream>

#include "ptc_mesh.hpp"
#include "ptc_console.hpp"
#include "ptc_renderer.hpp"
#include "ptc_vertex.hpp"
#include <cstddef>
#include <vector>

Mesh::Mesh() {}
Mesh::Mesh(std::string name) { this->name = name; this->vertices = {}; this->indices = {}; }
Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<int> indices)
{
    this->name = name;
    this->vertices = vertices;
    this->indices = indices;
}

void Mesh::RegisterBuffers()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// Mesh Mesh::processModelCode(std::string modelCode)
// {
// 	std::vector<float> modelVertices;
// 	std::vector<float> modelTextureCoords;
// 	std::vector<float> modelNormals;
// 	std::vector<int> modelIndices;

// 	std::vector<FaceData> modelFaces;
// 	int faceValueIndex = 0;

// 	while (modelCode.size() > 0)
// 	{
// 		std::string currLine = modelCode.substr(0, modelCode.find_first_of('\n'));
// 		modelCode = modelCode.substr(currLine.size() + 1, modelCode.size() - currLine.size() + 1);
			
// 		const std::string lineType = currLine.substr(0, currLine.find_first_of(' '));
// 		currLine = currLine.substr(currLine.find_first_of(' ') + 1, currLine.size() - currLine.find_first_of(' ') + 1);

// 		FaceData currFaceData = {};
			
// 		// Process current line
// 		switch (lineType[0])
// 		{
// 		case 'v':
// 			switch (lineType[1])
// 			{
// 			case '\0':
// 				while (currLine.size() > 0)
// 				{
// 					modelVertices.push_back(std::stof(currLine.substr(0, currLine.find_first_of(' '))));
// 					if (currLine.find_first_of(' ') == -1) break;
// 					currLine = currLine.substr(currLine.find_first_of(' ') + 1, currLine.size() - currLine.find_first_of(' '));
// 				}
// 				break;
// 			case 't':
// 				while (currLine.size() > 0)
// 				{
// 					modelTextureCoords.push_back(std::stof(currLine.substr(0, currLine.find_first_of(' '))));
// 					if (currLine.find_first_of(' ') == -1) break;
// 					currLine = currLine.substr(currLine.find_first_of(' ') + 1, currLine.size() - currLine.find_first_of(' '));
// 				}
// 				break;
// 			case 'n':
// 				while (currLine.size() > 0)
// 				{
// 					modelNormals.push_back(std::stof(currLine.substr(0, currLine.find_first_of(' '))));
// 					if (currLine.find_first_of(' ') == -1) break;
// 					currLine = currLine.substr(currLine.find_first_of(' ') + 1, currLine.size() - currLine.find_first_of(' '));
// 				}
// 				break;
// 			default:
// 				break;
// 			}
// 			break;
// 		case 'f':
// 			while (currLine.size() > 0)
// 			{
// 				if (faceValueIndex > 3)
// 				{
// 					SDL_Log("Loading too much data per face!\nModels need to be triangulated to work for now!!!");
// 					exit(-1);
// 				}
// 				std::string currFace = currLine.substr(0, currLine.find_first_of(' '));
					
// 				currFaceData.vertices[faceValueIndex] = std::stoi(currFace.substr(0, currFace.find_first_of('/')));
// 				currFaceData.textureCoordinates[faceValueIndex] = std::stoi(currFace.substr(currFace.find_first_of('/') + 1, currFace.find_last_of('/') - currFace.find_first_of('/') + 1));
// 				currFaceData.normals[faceValueIndex] = std::stoi(currFace.substr(currFace.find_last_of('/') + 1, currFace.size() - currFace.find_last_of('/') + 1));

// 				if (currLine.find_first_of(' ') == -1) break;
// 				currLine = currLine.substr(currLine.find_first_of(' ') + 1, currLine.size() - currLine.find_first_of(' '));
// 				faceValueIndex++;
// 			}
// 			faceValueIndex = 0;
// 			modelFaces.push_back(currFaceData);
// 			break;
// 		default:
// 			continue;
// 		}

// 	}

// 	std::vector<float> modelCombinedVertexBuffer;

// 	for (int i = 0; i < modelFaces.size(); i++)
// 	{
// 		for (int j = 0; j < 3; j++)
// 		{
// 			size_t vertexValue = static_cast<size_t>((modelFaces[i].vertices[j] - 1) * 3);
// 			size_t textureValue = static_cast<size_t>((modelFaces[i].textureCoordinates[j] - 1) * 2);
// 			size_t normalValue = static_cast<size_t>((modelFaces[i].normals[j] - 1) * 3);

// 			modelCombinedVertexBuffer.push_back(modelVertices[vertexValue]);
// 			modelCombinedVertexBuffer.push_back(modelVertices[vertexValue + 1]);
// 			modelCombinedVertexBuffer.push_back(modelVertices[vertexValue + 2]);

// 			modelCombinedVertexBuffer.push_back(modelTextureCoords[textureValue]);
// 			modelCombinedVertexBuffer.push_back(modelTextureCoords[textureValue + 1]);

// 			modelCombinedVertexBuffer.push_back(modelNormals[normalValue]);
// 			modelCombinedVertexBuffer.push_back(modelNormals[normalValue + 1]);
// 			modelCombinedVertexBuffer.push_back(modelNormals[normalValue + 2]);
// 		}
// 	}
		
// 	return Mesh((int)modelCombinedVertexBuffer.size(), modelCombinedVertexBuffer.data(), 0, nullptr);
// }
// Mesh::Mesh()
// {
// 	vertexCount = 0;
// 	visible = false;
// 	renderType = RenderType::NORMAL;
// 	VBO = 0;
// 	addMesh(loadModel("cube.obj"));
// }
// Mesh::Mesh(const char* _modelPath, RenderType _renderType)
// {
// 	vertexCount = 0;
// 	visible = false;
// 	renderType = _renderType;
// 	VBO = 0;
// 	addMesh(loadModel(_modelPath));
// }
// Mesh::Mesh(int _vertexCount, float* _vertices, int _indexCount, int* _indices, RenderType _renderType)
// {
// 	vertexCount = _vertexCount;
// 	visible = true;
// 	renderType = _renderType;

// 	glGenBuffers(1, &VBO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(float), _vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
// void Mesh::addMesh(Mesh msh)
// {
// 	vertexCount = msh.vertexCount;
// 	visible = msh.visible;
// 	VBO = msh.VBO;
// }
// void Mesh::drawInstance()
// {
// 	if (!visible || !enabled || !parentObject->enabled) return;
// 	Material* mat = GetComponent<Material>();

// 	mat->shader.use();
// 	GLint polygonMode[2];
// 	glGetIntegerv(GL_POLYGON_MODE, polygonMode);
// 	if (mat->shader.renderAsWireframe || Renderer::renderMode == GL_LINE)
// 	{
// 		glDisable(GL_CULL_FACE);
// 		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 	}

// 	glBindVertexArray(Renderer::VAO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);

// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);

// 	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
// 	glEnableVertexAttribArray(2);

// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, mat->texture);
// 	mat->shader.setInt("Texture", 0);
// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, mat->specularMap);
// 	mat->shader.setInt("SpecularMap", 1);
// 	glActiveTexture(GL_TEXTURE2);
// 	glBindTexture(GL_TEXTURE_2D, mat->normalMap);
// 	mat->shader.setInt("NormalMap", 2);

// 	glm::mat4 model = glm::mat4(1.0f);
// 	model = glm::translate(model, parentObject->transform.position);
	
// 	Object* cam = Object::Find("camera");
// 	Camera* camComp = cam->GetComponent<Camera>();
// 	glm::mat4 billboardRotation(1.0f);
// 	switch (renderType) { // Implement the other 2 billboard types!
// 		case RenderType::NORMAL:
// 			model *= glm::mat4_cast(parentObject->transform.rotation);
// 			break;
// 		case RenderType::CAMERA_FACING_BILLBOARD:
// 			model *= glm::mat4(glm::quatLookAt(glm::normalize(cam->transform.position - parentObject->transform.position), cam->GetComponent<Camera>()->WorldUp));
// 			break;
// 		case RenderType::Y_AXIS_ONLY_BillBOARD:
// 			model *= glm::mat4(glm::quatLookAt(glm::normalize(glm::vec3(cam->transform.position.x, parentObject->transform.position.y, cam->transform.position.z) - parentObject->transform.position), cam->GetComponent<Camera>()->WorldUp));
// 			break;
// 		default:
// 			model *= glm::mat4_cast(parentObject->transform.rotation);
// 			break;
// 	}

// 	model = glm::scale(model, parentObject->transform.scale);
// 	mat->shader.setMatrix4x4("model", model);
// 	mat->shader.setFloat4("baseCol", (mat->shader.renderAsWireframe || polygonMode[1] == GL_LINE) ? glm::vec4(glm::vec3(0.0f), 1.0f) : (GetComponent<Light>() != nullptr) ? glm::vec4(GetComponent<Light>()->lightColor, 1.0f) * mat->baseColor : mat->baseColor);
// 	mat->shader.setFloat("specularStrength", GetComponent<Material>()->specularStrength);
// 	mat->shader.setInt("specularExponent", GetComponent<Material>()->specularExponent);
// 	mat->shader.setFloat3("ambientLight", Light::ambientLight * glm::vec3(Light::ambientLightIntensity));

// 	mat->shader.setInt("lightNum", (int)Light::GetLights(Point).size());
// 	mat->shader.setFloat3v("lightPos", (GLsizei)Light::GetLights(Point).size(), Light::GetLightPositions(Point));
// 	mat->shader.setFloat3v("lightCol", (GLsizei)Light::GetLights(Point).size(), Light::GetLightColors(Point));
	
// 	mat->shader.setInt("dirLightNum", (int)Light::GetLights(Directional).size());
// 	mat->shader.setFloat3v("dirLightDir", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightDirections(Directional));
// 	mat->shader.setFloat3v("dirLightCol", (GLsizei)Light::GetLights(Directional).size(), Light::GetLightColors(Directional));

// 	mat->shader.setInt("spotLightNum", (int)Light::GetLights(Spot).size());
// 	mat->shader.setFloat3v("spotLightDir", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightDirections(Spot));
// 	mat->shader.setFloat3v("spotLightPos", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightPositions(Spot));
// 	mat->shader.setFloat3v("spotLightCol", (GLsizei)Light::GetLights(Spot).size(), Light::GetLightColors(Spot));
// 	mat->shader.setFloat1v("spotLightCutoff", (GLsizei)Light::GetSpotlightCutoff().size(), Light::GetSpotlightCutoff());
// 	mat->shader.setFloat1v("spotLightFocus", (GLsizei)Light::GetSpotlightFocus().size(), Light::GetSpotlightFocus());

// 	mat->shader.setFloat3("cameraPos", Camera::main->parentObject->transform.position);
		
// 	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);
// 	glEnable(GL_CULL_FACE);
// }
// Mesh Mesh::createEmptyMesh()
// {
// 	return Mesh(0, nullptr, 0, nullptr);
// }
// Mesh Mesh::loadModel(const char* modelPath)
// {
// 	if (*modelPath == '\0') {
// 		SDL_Log("No model!");
// 		return createEmptyMesh();
// 	}

// 	std::string fullModelPath = MODEL_PATH + std::string(modelPath);
// 	const char* fileExtension = strrchr(fullModelPath.c_str(), '.') + 1;
// 	if (!fileExtension)
// 	{
// 		SDL_Log("ERROR::MESH::MODEL_FILE_MISSING_EXTENSION");
// 		exit(-1);
// 	}

// 	if (std::string(fileExtension) != "obj")
// 	{
// 		SDL_Log("ERROR::MESH::UNSUPPORTED_MODEL_FILE_EXTENSION \".%s\"", fileExtension);
// 		exit(-1);
// 	}

// 	std::string modelCode = FileReader::Read(fullModelPath);

// 	return processModelCode(modelCode);
// }