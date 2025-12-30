#ifndef PTC_MESH_HPP
#define PTC_MESH_HPP
#include "ptc_vertex.hpp"
#include <string>
#include <vector>
constexpr char MODEL_PATH[] = "resources/models/";

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "ptc_renderer.hpp"

class Mesh
{
private:
	GLuint VBO, EBO;
public:
	GLuint VAO;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::string name;
	int smoothing;

	Mesh();
	Mesh(std::string name);
	Mesh(std::string name, std::vector<Vertex> vertices, std::vector<int> indices);

	void RegisterBuffers();
};

#endif