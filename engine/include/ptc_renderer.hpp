#ifndef PTC_RENDERER_HPP
#define PTC_RENDERER_HPP

#include "ptc_gui_structs.hpp"
#define GL_CHECK_ERROR()                                          \
    do {                                                          \
        GLenum err;                                               \
        while ((err = glGetError()) != GL_NO_ERROR) {             \
            fprintf(stderr, "OpenGL error 0x%X at %s:%d\n",       \
                    err, __FILE__, __LINE__);                     \
        }                                                         \
    } while (0)

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "ptc_camera.hpp"

class Renderer
{
public:
	static GLuint VAO;
	static GLuint UBO;
	static SDL_Window* window;
	static SDL_GLContext glContext;
	static Rect screen;
	static Rect viewport;
	static int FPSLimit;
	static GLint renderMode;

	Renderer();
	static void initSDL(const char* windowName = "Unnamed Window", int inScreenWidth = -1, int inScreenHeight = -1);
	static void initOpenGL();
	static void initRenderer();
	static void showWindow();
	static void prepareUI(Camera* camera);
	static void prepareFrame(Camera* camera);
	static void wrapFrame();
	static void SetViewport(Rect rect);
};

#endif