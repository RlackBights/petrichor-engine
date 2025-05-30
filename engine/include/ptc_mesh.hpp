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
	GLuint VBO, VAO, EBO;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
public:
	std::string name;

	Mesh();
	Mesh(std::string name);
	Mesh(std::string name, std::vector<Vertex> vertices, std::vector<int> indices);

	void AddVertex(Vertex vertex);
	void AddIndex(int index);
};

#endif