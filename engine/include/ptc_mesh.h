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

typedef enum RenderType {
	NORMAL,
	CAMERA_FACING_BILLBOARD,
	Y_AXIS_ONLY_BillBOARD,
	VIEW_ALIGNED_BILLBOARD,
	FIXED_SIZE_VIEW_ALIGNED_BILLBOARD,
} RenderType;

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
	RenderType renderType;

	Mesh();
	Mesh(const char* _modelPath, RenderType _renderType = RenderType::NORMAL);
	Mesh(int _vertexCount, float* _vertices, int _indexCount, int* _indices, RenderType _renderType = RenderType::NORMAL);
	void FixedUpdate() override;
	void addMesh(Mesh msh);
	void drawInstance();
	static Mesh createEmptyMesh();
	static Mesh loadModel(const char* modelPath = "");
};

#endif