#include <ptc_renderer.h>

Renderer::Renderer() {
	VAO = 0;
	glContext = 0;
}
void Renderer::initSDL(const char* windowName, int inScreenWidth, int inScreenHeight)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	Console::Write("--");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	Console::Write("--");

	const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(*SDL_GetDisplays(nullptr));
	screenWidth = (inScreenWidth <= 0) ? (int)(DM->w * 0.8f) : inScreenWidth;
	screenHeight = (inScreenHeight <= 0) ? (int)(DM->h * 0.8f) : inScreenHeight;
	Console::Write("--");


	// Create window
	window = SDL_CreateWindow(
		windowName,
		screenWidth,
		screenHeight,
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
	Console::Write("--");
}
void Renderer::showWindow()
{
	SDL_ShowWindow(window);
}
void Renderer::prepareFrame(Camera* camera)
{
	glClearColor(camera->backgroundColor.r, camera->backgroundColor.g, camera->backgroundColor.b, camera->backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);

	for (Shader shader : Shader::shaders)
	{
		shader.use();

		shader.setUInt("time", (GLuint)Time::currentFrame);
		shader.setMatrix4x4("view", camera->GetViewMatrix());
		shader.setMatrix4x4("projection", camera->GetProjectionMatrix(Renderer::screenWidth, Renderer::screenHeight));
	}

	glViewport(0, 0, screenWidth, screenHeight);
}
void Renderer::wrapFrame()
{
	glBindVertexArray(0);
	SDL_GL_SwapWindow(Renderer::window);
}

GLuint Renderer::UBO;
GLuint Renderer::VAO;
SDL_Window* Renderer::window;
SDL_GLContext Renderer::glContext;
int Renderer::screenWidth, Renderer::screenHeight;
int Renderer::FPSLimit;
GLint Renderer::renderMode;