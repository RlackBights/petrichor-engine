#include <string>
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ptc_component.h>
#include <ptc_shader.h>
#include <ptc_time.h>
#include <ptc_input.h>
#include <ptc_camera.h>
#include <ptc_text.h>
#include <ptc_mesh.h>
#include <ptc_renderer.h>
#include <ptc_texture.h>
#include <ptc_transform.h>
#include <ptc_console.h>

// Variables
Object root("root"), camera("camera"), cube("cube"), monkey("monkey"), sword("sword"), light("light"), cargo("cargo"), test_billboard("test_billboard"), light2("light2"), light3("light3");

// Function Definitions
bool init();
void initKeybinds();
bool update();
void cleanup(int exitCode = 0);

int main( int argc, char* argv[] )
{
	if (!init()) cleanup(-1);
	while (update());
	cleanup(0);
}

bool init()
{
	// Initialize rendering stuff
	Console::ClearScreen();
	Console::WriteLine("Initializing...", BRIGHT_YELLOW);
	Console::WriteLine("[          ]\tSDL");
	Console::SetCursorPosition(1, 1);
	Renderer::initSDL("PTC Framework");

	Console::WriteLine("\n[          ]\tOpenGL");
	Console::SetCursorPosition(1, 2);

	Renderer::initOpenGL();
	
	Console::WriteLine("\n[          ]\tCommon Functions shader");
	Console::SetCursorPosition(1, 3);
	
	Shader::SetCommonFunctionsShader("common_functions.glsl");

	Console::WriteLine("\n[          ]\tText manager");
	Console::SetCursorPosition(1, 4);

	TextManager::initTextManager(Shader("text_vert.glsl", "text_frag.glsl"), Renderer::screenWidth, Renderer::screenHeight);

	Console::WriteLine("\n[          ]\tCamera");
	Console::SetCursorPosition(1, 5);

	camera.AddComponent<Camera>(true);
	Console::Write("----------");

	Console::WriteLine("\n[          ]\tModels");
	Console::SetCursorPosition(1, 6);

	cube.AddComponent<Mesh>("cube.obj");
	Material* cubeMat = cube.AddComponent<Material>(Texture::loadTexture("house.png"), Texture::loadTexture("container2_specular.png"));
	cubeMat->specularExponent = 10;
	cubeMat->specularStrength = 2;
	cube.transform.position = glm::vec3(3.0f, 0.0f, 2.0f);
	Console::Write("--");

	Light::ambientLight = glm::vec4(0.7f, 0.8f, 0.5f, 1.0f);
	Light::ambientLightIntensity = 0.05f;

	light.AddComponent<Mesh>("orb.obj");
	Material* lightMat = light.AddComponent<Material>();
	lightMat->shader = Shader("vertex_shader.glsl", "flat_frag.glsl");
	light.AddComponent<Light>(Point, glm::vec3(0.7f, 0.8f, 0.5f));
	light.transform.position.y = 2.5f;

	Console::Write("--");

	light2.AddComponent<Mesh>("arrow.obj");
	Material* light2Mat = light2.AddComponent<Material>();
	light2Mat->shader = Shader("vertex_shader.glsl", "flat_frag.glsl");
	light2.AddComponent<Light>(Directional, glm::vec3(0.3f, 0.3f, 1.0f));
	light2.transform.rotation = glm::quat(glm::radians(glm::vec3(-25.0f, 45.0f, 0.0f)));
	light2.transform.scale = glm::vec3(0.2f);

	light3.AddComponent<Mesh>("cone.obj");
	Material* light3Mat = light3.AddComponent<Material>();
	light3Mat->shader = Shader("vertex_shader.glsl", "flat_frag.glsl");
	light3.AddComponent<Light>(Spot, glm::vec3(0.8f));
	light3.transform.scale = glm::vec3(0.2f);

	root.transform.AddChild(&light3.transform);

	test_billboard.AddComponent<Mesh>("plane.obj");
	test_billboard.GetComponent<Mesh>()->isBillboard = true;
	test_billboard.AddComponent<Material>(Texture::loadTexture("house.png"));
	test_billboard.transform.position.x = 3.0f;

	test_billboard.enabled = false;
	Console::Write("--");

	monkey.AddComponent<Mesh>("monkey.obj");
	monkey.AddComponent<Material>(Texture::loadTexture("wall.jpg"));
	monkey.transform.position = glm::vec3(3.0f, 0.0f, -2.0f);
	monkey.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));
	Console::Write("--");

	sword.AddComponent<Mesh>("EthanSword.obj");
	sword.AddComponent<Material>(Texture::loadTexture("container.png"));
	sword.transform.position = glm::vec3(3.0f, 2.0f, 0.0f);
	sword.transform.scale = glm::vec3(5.0f);
	Console::Write("--");
	
	Console::WriteLine("\n[          ]\tInput");
	Console::SetCursorPosition(1, 7);

	Input::initInput(&Renderer::screenWidth, &Renderer::screenHeight);
	Console::Write("-----");
	initKeybinds();

	Console::WriteLine("\n[          ]\tRenderer");
	Console::SetCursorPosition(1, 8);

	Renderer::initRenderer();

	Console::WriteLine("\n[          ]\tTime");
	Console::SetCursorPosition(1, 9);

	Time::initTime();
	Console::Write("----------");

	Console::WriteLine("\nFinished initialization!");

	Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) comp->Start(); } );

	Console::ClearFormatting();
	Renderer::showWindow();
	return true;
}

bool update()
{
	// Initialize stuff for the frame
	Time::updateTime();
	Input::updateInputUnscaled();
	Renderer::prepareFrame(camera.GetComponent<Camera>());

	// static float time = 0;
	// time += Time::deltaTime * 2;
	// cube.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, time * 100.0f, 0.0f)));
	// light.transform.position = glm::vec3(cos(time) * 7, 0.0f, sin(time) *7);
	// Transform camTrans = Camera::getMainCamera()->parentObject->transform;
	// light3.transform.position = camTrans.position;
	// light3.transform.rotation = camTrans.rotation;
	TextManager::renderText(std::to_string((int)glm::round(1 + Time::timeScale / Time::deltaTime)) + " FPS", 25.0f, 25.0f, 0.5f, Renderer::screenWidth, Renderer::screenHeight, glm::vec3(1.0f));



	Time::fixedTimeAccumulator += Time::deltaTime;
	int fixedUpdateCount = 0;

	while (Time::fixedTimeAccumulator >= Time::fixedDeltaTime)
	{
		fixedUpdateCount++;
		Time::fixedTimeAccumulator -= Time::fixedDeltaTime;
	}

	Transform::GetRoot()->PreorderTraversal([&](Transform* node) {
		for (const auto& comp : *node->object->GetComponents())
		{
			for (int i = 0; i < fixedUpdateCount; i++) comp->FixedUpdate();
			comp->Update();
		}
	});

	Time::wrapTime();
	Renderer::wrapFrame();
	Input::wrapInput();
	return true;
}

void cleanup(int exitCode)
{
	// Cleanup
	SDL_GL_DestroyContext(Renderer::glContext);
	SDL_DestroyWindow(Renderer::window);
	SDL_Quit();
	exit(exitCode);
}

void initKeybinds()
{
	Input::addBinding(SDLK_ESCAPE, KEYBIND_DOWN, []() { cleanup(1); }, true);
	Input::addBinding(SDLK_W, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(FORWARD, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_A, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(LEFT, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_S, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(BACKWARD, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_D, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(RIGHT, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_SPACE, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(UP, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_LALT, KEYBIND_HOLD, []() { camera.GetComponent<Camera>()->MoveCamera(DOWN, Time::deltaTimeUnscaled); });
	Input::addBinding(SDLK_LSHIFT, KEYBIND_DOWN, []() { camera.GetComponent<Camera>()->isBoosting = true; });
	Input::addBinding(SDLK_LSHIFT, KEYBIND_UP, []() { camera.GetComponent<Camera>()->isBoosting = false; });
	Input::addBinding(SDLK_F11, KEYBIND_DOWN, []() { SDL_SetWindowFullscreen(Renderer::window, !(SDL_GetWindowFlags(Renderer::window) & SDL_WINDOW_FULLSCREEN)); }, true);
	Input::addBinding(SDLK_V, KEYBIND_DOWN, []() { glPolygonMode(GL_FRONT_AND_BACK, (Renderer::renderMode == GL_LINE) ? Renderer::renderMode = GL_FILL : Renderer::renderMode = GL_LINE); if (Renderer::renderMode == GL_LINE) glDisable(GL_CULL_FACE); else glEnable(GL_CULL_FACE); }, true);

	Input::addBinding(SDLM_RIGHT, MOUSE_DOWN, []() {
		Input::enabled = true;
		SDL_SetWindowRelativeMouseMode(Renderer::window, true);
	}, true);

	Input::addBinding(SDLM_RIGHT, MOUSE_UP, []() {
		Input::enabled = false;
		SDL_SetWindowRelativeMouseMode(Renderer::window, false);
		SDL_WarpMouseInWindow(Renderer::window, Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
	}, true);

	Input::enabled = false;
	Console::Write("-----");
}