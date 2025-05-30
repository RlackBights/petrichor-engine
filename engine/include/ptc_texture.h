#ifndef PTC_TEXTURE_H
#define PTC_TEXTURE_H
#include <vector>
constexpr char TEXTURE_PATH[] = "resources/textures/";

#include <string>
#include <glad/glad.h>
#include <stb_image.h>
#include <SDL3/SDL.h>

class Texture
{
private:
	static int width, height, nrChannels;
	static unsigned char* data;
	static std::vector<std::string> usedPaths;
public:
	static std::vector<GLuint> textures;
	static GLuint loadTexture(std::string texturePath);
};

#endif