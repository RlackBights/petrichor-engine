#ifndef PTC_MESH_H
#define PTC_MESH_H
constexpr char MODEL_PATH[] = "resources/models/";

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include <ptc_renderer.h>
#include <ptc_transform.h>
#include <ptc_camera.h>
#include <ptc_light.h>
#include <ptc_texture.h>
#include <ptc_material.h>
#include <ptc_component.h>

typedef struct FaceData {
	int vertices[3];
	int textureCoordinates[3];
	int normals[3];
} FaceData;

class Mesh : public Component
{
private:
	GLuint VBO;
	static PointLight* pointLights;
	static int lightNum;
	static Mesh processModelCode(std::string modelCode);

public:
	int vertexCount;
	bool visible;
	bool isBillboard;

	Mesh();
	Mesh(const char* modelPath, bool _isBillboard = false);
	Mesh(int inVertexCount, float* inVertices, int inIndexCount, int* inIndices, bool _isBillboard = false);
	void addMesh(Mesh msh);
	void drawInstance();
	static Mesh createEmptyMesh();
	static Mesh loadModel(const char* modelPath = "");
};

#endif