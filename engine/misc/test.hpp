#include "../include/ptc_engine.hpp"
#define _Bool bool

class GeneratedClassDefinitions
{
private:
	MeshFilter * filterRef;
	std::string text;
	glm::vec2 position;
	glm::vec2 offset;
	unsigned int VAO;
	unsigned int VBO;
	Font * font;
	glm::vec4 color;
	std::function<float (float)> animationFunction;
protected:
// Camera
// Light
// Material
// MeshFilter
// MeshRenderer
// Text
public:
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec4 backgroundColor;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	_Bool isBoosting;
	_Bool perspective;
	glm::vec3 lightColor;
	LightType type;
	float cutoff;
	float focus;
	glm::vec4 baseColor;
	GLuint texture;
	GLuint specularMap;
	GLuint normalMap;
	Shader shader;
	float specularStrength;
	int specularExponent;
	Mesh mesh;
	Material material;
	Shader textShader;
	static void ProcessToken(const char* _token)
	{
		return;
	}
};