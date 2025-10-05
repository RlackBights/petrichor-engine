
#include "glm/fwd.hpp"
#include "ptc_console.hpp"
#include "ptc_engine.hpp"
#include "./standalone/example_standalone.cpp"
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

Object camera("camera"), test("test"), test2("test2"), text("text"), idk("idk"), light("light");
void game_main()
{
    Camera* ref = camera.AddComponent<Camera>();
    Mesh cube = MeshLoader::LoadMesh("Bench.obj")["Cylinder"];
    Mesh cube2 = MeshLoader::LoadMesh("Bench.obj")["Cylinder.001"];
    test2.AddComponent<MeshFilter>(cube2);
    test2.AddComponent<MeshRenderer>();
    test.AddComponent<MeshFilter>(cube);
    MeshRenderer* rf = test.AddComponent<MeshRenderer>();
    rf->material.texture = Texture::loadTexture("house.png");

    // Mesh sphere = MeshLoader::LoadMesh("Sphere.obj")["Sphere"];
    // light.AddComponent<MeshFilter>(sphere);
    // light.AddComponent<MeshRenderer>();
    // Light* lightRef = light.AddComponent<Light>(LightType::Point);
    // light.transform.position = glm::vec3(0, 1, 0);

    JSONValue json = JSON::Parse(FileProcessor::Read("resources/other/example.json"));

    int idk2[] = {1, 2, 0, 3, 5};
    Console::WriteLine(json);

    std::vector<uint8_t> bytes = FileProcessor::ReadBytes("resources/models/Cube.glb");
    auto _text = FileProcessor::ProcessBytes<char>(std::vector<uint8_t>(bytes.begin(),bytes.begin() + 4));
    for (int i = 0; i < 100; i++) {
        Debug::Log(std::string(_text.begin(), _text.end()));
        Debug::Warn((std::vector<int>){1, 2, 0, 3, 5});
        Debug::Error("Woah");
    }

    test.transform.AddChild(&idk.transform);

    test.AddComponent<ExampleStandaloneComponent>();

}