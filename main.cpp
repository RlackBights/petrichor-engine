#include "ptc_gui.h"
#include "ptc_state.h"
#include <SDL3/SDL_keycode.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <string>
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scripts/game_main.cpp"
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

// Function Definitions
bool init();
void initKeybinds();
bool update();
void cleanup(int exitCode = 0);

int main( int argc, char* argv[] )
{
	State::SetArguments(argc, argv);
	if (!init()) cleanup(-1);
	while (update());
	cleanup(0);
}

bool init()
{
	int consoleOffset = 1;

	// Initialize rendering stuff
	Console::ClearScreen();
	Console::WriteLine("Initializing...", BRIGHT_YELLOW);
	Console::WriteLine("[          ]\tSDL");
	Console::SetCursorPosition(1, consoleOffset++);
	Renderer::initSDL("PTC Framework");

	Console::WriteLine("\n[          ]\tOpenGL");
	Console::SetCursorPosition(1, consoleOffset++);

	Renderer::initOpenGL();
	
	Console::WriteLine("\n[          ]\tCommon Functions shader");
	Console::SetCursorPosition(1, consoleOffset++);
	
	Shader::SetCommonFunctionsShader("common_functions.glsl");

	Console::WriteLine("\n[          ]\tCamera");
	Console::SetCursorPosition(1, consoleOffset++);

	Console::Write("----------");
	
	Console::WriteLine("\n[          ]\tInput");
	Console::SetCursorPosition(1, consoleOffset++);

	Input::initInput(&Renderer::screenWidth, &Renderer::screenHeight);
	Console::Write("-----");
	initKeybinds();

	Console::WriteLine("\n[          ]\tRenderer");
	Console::SetCursorPosition(1, consoleOffset++);

	Renderer::initRenderer();
	GUI::InitUI();
	Light::ambientLight = glm::vec4(0.7f, 0.8f, 0.5f, 1.0f);
	Light::ambientLightIntensity = 0.05f;

	Console::WriteLine("\n[          ]\tTime");
	Console::SetCursorPosition(1, consoleOffset++);

	Time::initTime();
	Console::Write("----------");

	Console::WriteLine("\nFinished initialization!");

	game_main();
	Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) if (comp->enabled && comp->parentObject->enabled) comp->Start(); } );

	Console::ClearFormatting();
	Renderer::showWindow();
	return true;
}

bool update()
{
	// Need rendering passes separately for the engine UI, and for the game preview
	// Need a hierarchy (Should be easy since the nodes already exist), an inspector (sounds like absolute hell), and a console (idk)
	// Might want a file manager too, but it can wait
	// Engine-level features required:
	// Sound system (Probably the easiest)
	// Fix 3d rendering (Not too hard, but quite a bit of work)
	// Add bone-based animation support (Sounds like absolute hell unless there's some very obvious way how to handle it)
	// Add a particle system  (Heard a bunch about it, probably not the worst)
	// Scripting language??? Maybe add Lua(4/10)/Python(8/10)/Js(6/10)

	// ENGINE

	Renderer::prepareUI(Camera::main);
	GUI::RenderUI();

	// GAME

	// Initialize stuff for the frame
	Time::updateTime();
	Input::updateInputUnscaled();
	Renderer::prepareFrame(Camera::main ? Camera::main : nullptr);
	
	// Run the update functions
	Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) if (comp->enabled && comp->parentObject->enabled) comp->Update(); } );
	
	// Check if we are running at the frame limit
	if (!Time::isNextFrameReady(Renderer::FPSLimit)) return true;
		
	// Run the fixedUpdate functions
	Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) if (comp->enabled && comp->parentObject->enabled) comp->FixedUpdate(); } );

	// Frame cleanup
	Time::wrapTime();
	Input::wrapInput();
	Renderer::wrapFrame();

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
	Input::addBinding(SDLK_ESCAPE, KEYBIND_DOWN, []() { cleanup(1); }, true); // Closes the application on escape
	// Switches between fullscreen and windowed mode on F11
	Input::addBinding(SDLK_F11, KEYBIND_DOWN, []() { SDL_SetWindowFullscreen(Renderer::window, !(SDL_GetWindowFlags(Renderer::window) & SDL_WINDOW_FULLSCREEN)); }, true);

	// These act as the engine camera controls:

	// Input::addBinding(SDLK_W, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(FORWARD, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_A, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(LEFT, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_S, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(BACKWARD, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_D, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(RIGHT, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_SPACE, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(UP, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_LALT, KEYBIND_HOLD, []() { Camera::main->GetComponent<Camera>()->MoveCamera(DOWN, Time::deltaTimeUnscaled); });
	// Input::addBinding(SDLK_LSHIFT, KEYBIND_DOWN, []() { Camera::main->GetComponent<Camera>()->isBoosting = true; });
	// Input::addBinding(SDLK_LSHIFT, KEYBIND_UP, []() { Camera::main->GetComponent<Camera>()->isBoosting = false; });

	// Input::addBinding(SDLM_RIGHT, MOUSE_DOWN, []() 
	// 	Input::enabled = true;
	// 	SDL_SetWindowRelativeMouseMode(Renderer::window, true);
	// }, true);

	// Input::addBinding(SDLM_RIGHT, MOUSE_UP, []() {
	// 	Input::enabled = false;
	// 	SDL_SetWindowRelativeMouseMode(Renderer::window, false);
	// 	SDL_WarpMouseInWindow(Renderer::window, Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
	// }, true);


	
	// Debug features, wireframe rendering and switch between orthographic and perspective rendering:
	
	// Input::addBinding(SDLK_V, KEYBIND_DOWN, []() { glPolygonMode(GL_FRONT_AND_BACK, (Renderer::renderMode == GL_LINE) ? Renderer::renderMode = GL_FILL : Renderer::renderMode = GL_LINE); if (Renderer::renderMode == GL_LINE) glDisable(GL_CULL_FACE); else glEnable(GL_CULL_FACE); }, true);
	// Input::addBinding(SDLK_N, KEYBIND_DOWN, []() { Camera::main->perspective = !Camera::main->perspective; });

	Input::enabled = true; // Was originally false, since the camera movement was locked unless the user held down the right mouse button, similar to unity
	Console::Write("-----");
}