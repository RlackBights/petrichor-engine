
#include "ptc_camera.hpp"
#include "ptc_file_reader.hpp"
#include "ptc_json.hpp"
#include "ptc_mesh.hpp"
#include "ptc_mesh_filter.hpp"
#include "ptc_mesh_loader.hpp"
#include "ptc_mesh_renderer.hpp"
#include "ptc_object.hpp"
#include "./standalone/example_standalone.cpp"
#include "ptc_texture.hpp"
Object camera("camera"), test("test"), test2("test2"), text("text");
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

    JSON::Parse(FileReader::Read("resources/other/example.json"));
}