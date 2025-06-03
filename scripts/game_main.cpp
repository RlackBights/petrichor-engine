
#include "ptc_camera.hpp"
#include "ptc_console.hpp"
#include "ptc_file_reader.hpp"
#include "ptc_json.hpp"
#include "ptc_json_structs.hpp"
#include "ptc_mesh.hpp"
#include "ptc_mesh_filter.hpp"
#include "ptc_mesh_loader.hpp"
#include "ptc_mesh_renderer.hpp"
#include "ptc_object.hpp"
#include "./standalone/example_standalone.cpp"
#include "ptc_texture.hpp"
#include <cstdint>
#include <string>
#include <vector>
Object camera("camera"), test("test"), test2("test2"), text("text"), idk("idk");
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

    JSONValue json = JSON::Parse(FileReader::Read("resources/other/example.json"));

    std::vector<uint8_t> bytes = FileReader::ReadBytes("resources/models/Cube.glb");
    auto text = FileReader::ProcessBytes<char>(std::vector<uint8_t>(bytes.begin(),bytes.begin() + 4));
    Console::WriteLine(std::string(text.begin(), text.end()));

    test.transform.AddChild(&idk.transform);
}