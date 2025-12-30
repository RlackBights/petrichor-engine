#ifndef PTC_RENDERER_HPP
#define PTC_RENDERER_HPP

#include "SDL3/SDL_mouse.h"
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
private:
	static SDL_Cursor* cursors[SDL_SYSTEM_CURSOR_COUNT];
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
	static void prepareUI();
	static void prepareFrame();
	static void wrapFrame();
	static void SetViewport(Rect rect);
	static void SetCursor(SDL_SystemCursor cursor);
};

#endif