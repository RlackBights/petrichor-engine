#ifndef PTC_RENDERER_H
#define PTC_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include <ptc_console.h>
#include <ptc_light.h>
#include <ptc_shader.h>
#include <ptc_camera.h>
#include <ptc_time.h>

class Renderer
{
public:
	static GLuint VAO;
	static GLuint UBO;
	static SDL_Window* window;
	static SDL_GLContext glContext;
	static int screenWidth, screenHeight;
	static int FPSLimit;
	static GLint renderMode;

	Renderer();
	static void initSDL(const char* windowName = "Unnamed Window", int inScreenWidth = -1, int inScreenHeight = -1);
	static void initOpenGL();
	static void initRenderer();
	static void showWindow();
	static void prepareFrame(Camera* camera);
	static void wrapFrame();
};

#endif