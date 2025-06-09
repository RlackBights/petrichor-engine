#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_video.h"
#include "ptc_debug.hpp"
#include "ptc_debug_structs.hpp"
#include "ptc_gui.hpp"
#include "ptc_light.hpp"
#include "ptc_state.hpp"
#include <SDL3/SDL_keycode.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <map>
#include <string>
#include <utility>
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scripts/game_main.cpp"
#include <ptc_component.hpp>
#include <ptc_shader.hpp>
#include <ptc_time.hpp>
#include <ptc_input.hpp>
#include <ptc_camera.hpp>
#include <ptc_text.hpp>
#include <ptc_mesh.hpp>
#include <ptc_renderer.hpp>
#include <ptc_texture.hpp>
#include <ptc_transform.hpp>
#include <ptc_console.hpp>

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

	Input::initInput(&Renderer::screen.width, &Renderer::screen.height);
	Console::Write("-----");
	initKeybinds();

	Console::WriteLine("\n[          ]\tRenderer");
	Console::SetCursorPosition(1, consoleOffset++);

	Renderer::initRenderer();
	GUI::InitUI();
	Light::ambientLight = glm::vec4(1.0f);
	Light::ambientLightIntensity = 1.0f;

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

	Renderer::prepareUI();
	GUI::ApplyLayout();

	GUI::Begin("Hierarchy");
	
	Transform::GetRoot()->PreorderTraversal([](Transform* node) { 
		std::string hierarchyEntry = "";
		Transform* currNode = node->parent;
		while (currNode->parent) {
			hierarchyEntry += "|  ";
			currNode = currNode->parent;
		}
		hierarchyEntry += node->object->name;
		if (GUI::Button(hierarchyEntry, glm::vec2(Text::getStaticPixelWidth(hierarchyEntry, 20), 20), true))
		{
			Console::WriteLine("Inspector: " + node->object->name);
		}
	});

	GUI::End();

	GUI::Begin("Console");

	if (GUI::Button("group", {300, 40}))
	{
		Debug::SwitchLogGrouping();
	}
	
	if (Debug::GetLogGrouping()) {
		std::map<std::pair<LogType, std::string>, int> groups = {};
		for (auto& log : Debug::GetLogs()) {
			bool exists = false;
			for (auto& [key, value] : groups) {
				if (log == key) 
				{
					exists = true;
					value++;
					break;
				}
			}
			if (!exists) groups.insert({log, 1});
		}

		for (auto& log : groups) {
			GUI::Label(((log.first.first == LogType::LOG_INFO) ? "Info: \t\t\t" : ((log.first.first == LogType::LOG_WARNING) ? "[ffff22]Warning: \t" : "[ff5555]Error: \t\t\t")) + log.first.second + Console::FormatString((log.second > 999) ? " (%d+)" : " (%d)", std::clamp(log.second, 1, 999)));
			GUI::Divider();
		}

	} else {
		for (auto& log : Debug::GetLogs()) {
			GUI::Label(((log.first == LogType::LOG_INFO) ? "Info: \t\t\t" : ((log.first == LogType::LOG_WARNING) ? "[ffff22]Warning: \t" : "[ff5555]Error: \t\t\t")) + log.second);
			GUI::Divider();
		}
	}

	GUI::End();

	GUI::Begin("Inspector");
	GUI::End();

	// GAME

	// Initialize stuff for the frame
	Time::updateTime();
	Input::updateInput();
	Renderer::prepareFrame();
	
	while (Time::fixedAccumulator >= Time::fixedUpdateFrametime)
	{
		// Run the fixedUpdate functions
		Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) if (comp->enabled && comp->parentObject->enabled) comp->FixedUpdate(); } );
		Time::fixedAccumulator -= 0.02f;
	}
	
	// Run the update functions
	Transform::GetRoot()->PreorderTraversal([](Transform* node) { for (const auto& comp : *node->object->GetComponents()) if (comp->enabled && comp->parentObject->enabled) comp->Update(); } );


	// Frame cleanup
	GUI::RenderUI();
	Time::wrapTime();
	Input::wrapInput();
	Renderer::wrapFrame();
	GUI::wrapFrame();

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
	//Input::addBinding("Close", SDLK_ESCAPE, KEYBIND_DOWN, []() { cleanup(1); }, true); // Closes the application on escape
	// Switches between fullscreen and windowed mode on F11
	Input::addBinding("Fullscreen", SDLK_F11, KEYBIND_DOWN, []() { SDL_SetWindowFullscreen(Renderer::window, !(SDL_GetWindowFlags(Renderer::window) & SDL_WINDOW_FULLSCREEN)); }, true);

	// These act as the engine camera controls:

	Input::addBinding("EditorCameraForward", SDLK_W, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(FORWARD, Time::deltaTime); });
	Input::addBinding("EditorCameraLeft", SDLK_A, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(LEFT, Time::deltaTime); });
	Input::addBinding("EditorCameraBackward", SDLK_S, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(BACKWARD, Time::deltaTime); });
	Input::addBinding("EditorCameraRight", SDLK_D, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(RIGHT, Time::deltaTime); });
	Input::addBinding("EditorCameraUp", SDLK_SPACE, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(UP, Time::deltaTime); });
	Input::addBinding("EditorCameraDown", SDLK_LALT, KEYBIND_HOLD, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->MoveCamera(DOWN, Time::deltaTime); });
	Input::addBinding("EditorCameraBoostStart", SDLK_LSHIFT, KEYBIND_DOWN, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->isBoosting = true; });
	Input::addBinding("EditorCameraBoostEnd", SDLK_LSHIFT, KEYBIND_UP, []() { if (Camera::main) Camera::main->GetComponent<Camera>()->isBoosting = false; });

	Input::addBinding("EditorFocusSceneStart", SDLM_RIGHT, MOUSE_DOWN, []() {
		if (!GUI::isHovered(Renderer::viewport) || !Camera::main) return;
		Input::enabled = true;
		Input::lastSceneMousePosition = glm::vec2(Input::mouseX, Input::mouseY);
		SDL_SetWindowRelativeMouseMode(Renderer::window, true);
	}, true);

	Input::addBinding("EditorFocusSceneEnd", SDLM_RIGHT, MOUSE_UP, []() {
		if (!SDL_GetWindowRelativeMouseMode(Renderer::window) || !Camera::main) return;
		Input::enabled = false;
		SDL_SetWindowRelativeMouseMode(Renderer::window, false);
		SDL_WarpMouseInWindow(Renderer::window, Input::lastSceneMousePosition.x, Input::lastSceneMousePosition.y);
	}, true);


	
	
	// Debug features, wireframe rendering and switch between orthographic and perspective rendering:
	
	Input::addBinding("EditorRenderWireframe", SDLK_V, KEYBIND_DOWN, []() { glPolygonMode(GL_FRONT_AND_BACK, (Renderer::renderMode == GL_LINE) ? Renderer::renderMode = GL_FILL : Renderer::renderMode = GL_LINE); if (Renderer::renderMode == GL_LINE) glDisable(GL_CULL_FACE); else glEnable(GL_CULL_FACE); });
	Input::addBinding("EditorSwitchPerspective", SDLK_N, KEYBIND_DOWN, []() { Camera::main->perspective = !Camera::main->perspective; });

	Input::enabled = false; // Was originally false, since the camera movement was locked unless the user held down the right mouse button, similar to unity
	Console::Write("-----");
}