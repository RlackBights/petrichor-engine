#include "SDL3/SDL_mouse.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "ptc_camera.hpp"
#include "ptc_console.hpp"
#include "ptc_gui_structs.hpp"
#include "ptc_shader.hpp"
#include "ptc_text.hpp"
#include "ptc_time.hpp"
#include <ptc_renderer.hpp>

Renderer::Renderer() {
	VAO = 0;
	glContext = 0;
}
void Renderer::initSDL(const char* windowName, int inScreenWidth, int inScreenHeight)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		Console::WriteLine(Console::FormatString("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()), Color::RED, false);
		return;
	}
	Console::Write("--");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	Console::Write("--");

	const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(*SDL_GetDisplays(nullptr));
	screen = Rect(0, 0, (inScreenWidth <= 0) ? (int)(DM->w * 0.8f) : inScreenWidth, (inScreenHeight <= 0) ? (int)(DM->h * 0.8f) : inScreenHeight);
	viewport = screen;
	Console::Write("--");


	// Create window
	window = SDL_CreateWindow(
		windowName,
		screen.width,
		screen.height,
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_HIDDEN
	);
	Console::Write("--");

	if (window == NULL)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	FPSLimit = (int)glm::round(SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay())->refresh_rate);
	Console::Write("--");
}
void Renderer::initOpenGL()
{
	// Create OpenGL context
	glContext = SDL_GL_CreateContext(window);
	Console::Write("---");

	if (glContext == NULL)
	{
		SDL_Log("ERROR::RENDERER::COULD_NOT_CREATE_OPENGL_CONTEXT");
		exit(-1);
	}
	Console::Write("---");

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_Log("ERROR::RENDERER::COULD_NOT_INIT_GLAD");
		exit(-1);
	}
	Console::Write("----");
}
void Renderer::initRenderer()
{
	renderMode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, renderMode);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	Console::Write("---");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	Console::Write("----");

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 1232, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, Renderer::UBO);

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Console::Write("---");
}
void Renderer::showWindow()
{
	SDL_ShowWindow(window);
}
void Renderer::prepareUI()
{
	glDisable(GL_SCISSOR_TEST);
	glViewport(0, 0, screen.width, screen.height);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::prepareFrame()
{
	glViewport(viewport.x,Renderer::screen.height - viewport.y - viewport.height, viewport.width, viewport.height);
	glScissor(viewport.x, Renderer::screen.height - viewport.y - viewport.height, viewport.width, viewport.height);
	glEnable(GL_SCISSOR_TEST);

	if (Camera::main) glClearColor(Camera::main->backgroundColor.r, Camera::main->backgroundColor.g, Camera::main->backgroundColor.b, Camera::main->backgroundColor.a);
	else glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);

	for (Shader shader : Shader::shaders)
	{
		shader.use();
		shader.setUInt("time", (GLuint)Time::currentFrame);
		shader.setMatrix4x4("view", (Camera::main) ? Camera::main->GetViewMatrix() : glm::mat4(1.0f));
		glm::mat4 projMatrix;
		if (Camera::main) projMatrix = Camera::main->GetProjectionMatrix(Renderer::viewport.width, Renderer::viewport.height);
		else projMatrix = glm::ortho(0.0f, (float)Renderer::viewport.width, (float)Renderer::viewport.height, 0.0f, -1.0f, 1.0f);
		shader.setMatrix4x4("projection", projMatrix);

		glUseProgram(0);
	}

	if (!Camera::main) Text::StaticDrawText("No Main Camera found!", glm::vec2(Renderer::screen.width / 2, Renderer::screen.height / 2), Renderer::viewport);

	glDisable(GL_SCISSOR_TEST);
}
void Renderer::wrapFrame()
{
	SDL_GL_SwapWindow(Renderer::window);
}
void Renderer::SetViewport(Rect rect)
{
	Renderer::viewport = rect;
}
void Renderer::SetCursor(SDL_SystemCursor cursor)
{
	if (!cursors[cursor]) {
        cursors[cursor] = SDL_CreateSystemCursor(cursor);
    }

    if (SDL_GetCursor() != cursors[cursor]) {
        SDL_SetCursor(cursors[cursor]);
    }
}

SDL_Cursor* Renderer::cursors[SDL_SYSTEM_CURSOR_COUNT];
GLuint Renderer::UBO;
GLuint Renderer::VAO;
SDL_Window* Renderer::window;
SDL_GLContext Renderer::glContext;
Rect Renderer::screen, Renderer::viewport;
int Renderer::FPSLimit;
GLint Renderer::renderMode;