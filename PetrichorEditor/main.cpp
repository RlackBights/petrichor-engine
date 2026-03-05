#include "Debug/Debug.h"
#include "Debug/DebugStructs.h"
#include "GUI/GUI.h"
#include "PetrichorInputAPI/InputManager.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Renderer.h"
#include "SDL3_OpenGL_Backend/InputBackend.h"
#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include "Time/Time.h"
#include <Core/Log.h>
#include <memory>
#include <string>
#include <unistd.h>
#include <utility>

void RenderEditorGUI()
{
    PetrichorEditor::GUI::ApplyLayout();

    PetrichorEditor::GUI::Begin("Hierarchy");
    PetrichorEditor::GUI::End();

    PetrichorEditor::GUI::Begin("Console");
    if (PetrichorEditor::GUI::Button("group", { 300, 40 }))
    {
        PetrichorEngine::Debug::SwitchLogGrouping();
    }

    if (PetrichorEngine::Debug::GetLogGrouping()) {
        std::map<std::pair<PetrichorEngine::LogType, std::string>, int> groups = {};
        for (auto& log : PetrichorEngine::Debug::GetLogs()) {
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
            PetrichorEditor::GUI::Label(((log.first.first == PetrichorEngine::LogType::LOG_INFO) ? "Info: \t\t\t" : ((log.first.first == PetrichorEngine::LogType::LOG_WARNING) ? "[ffff22]Warning: \t" : "[ff5555]Error: \t\t\t")) + log.first.second + ((log.second > 999) ? " (999+)" : " (" + std::to_string(log.second) + ")"));
            PetrichorEditor::GUI::Divider();
        }

    } else {
        for (auto& log : PetrichorEngine::Debug::GetLogs()) {
            PetrichorEditor::GUI::Label(((log.first == PetrichorEngine::LogType::LOG_INFO) ? "Info: \t\t\t" : ((log.first == PetrichorEngine::LogType::LOG_WARNING) ? "[ffff22]Warning: \t" : "[ff5555]Error: \t\t\t")) + log.second);
            PetrichorEditor::GUI::Divider();
        }
    }

	PetrichorEditor::GUI::End();

	PetrichorEditor::GUI::Begin("Inspector");
	PetrichorEditor::GUI::End();
	
	PetrichorEditor::GUI::Begin("Scene", false, false);
	PetrichorEditor::GUI::Label(std::to_string(1 / PetrichorEngine::Time::deltaTime));
	PetrichorEditor::GUI::End();
    
    PetrichorEditor::GUI::RenderGUI();
}

int main()
{
    auto rendererBackend = std::make_unique<RendererBackends::SDL3_OpenGL::RendererBackend>();
    PetrichorRendererAPI::Renderer::AssignBackend(std::move(rendererBackend));

    auto inputBackend = std::make_unique<RendererBackends::SDL3_OpenGL::InputBackend>();
    PetrichorInputAPI::InputManager::AssignInputManager(std::move(inputBackend));

    PetrichorRendererAPI::Renderer::InitializeWindow("Game!!!!");
    PetrichorInputAPI::InputManager::InitializeInput();

    PetrichorRendererAPI::Data::Rect r;
    PetrichorRendererAPI::Data::Material m;

    PetrichorEditor::GUI::InitializeGUI();

    while (1)
    {
        PetrichorRendererAPI::Renderer::InitializeFrame();
        PetrichorInputAPI::InputManager::UpdateInput();
        
        RenderEditorGUI();

        PetrichorInputAPI::InputManager::WrapInput();
        PetrichorEditor::GUI::WrapGUI();
        PetrichorRendererAPI::Renderer::WrapFrame();
    }

    return 0;
}