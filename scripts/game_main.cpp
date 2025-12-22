
#include "AABB_structs.hpp"
#include "./standalone/gui_stuff.cpp"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "ptc_engine.hpp"
#include "./standalone/example_standalone.cpp"
#include "ptc_mesh_filter.hpp"
#include "ptc_mesh_loader.hpp"
#include "ptc_mesh_renderer.hpp"
#include "ptc_object.hpp"
#include "ptc_texture.hpp"
#include <map>
#include <string>
#include <vector>

void game_main()
{
    Object camera("camera"), idk("idk");
    
    Light::ambientLightIntensity = 1.0f;
    Camera* ref = camera.AddComponent<Camera>();
    ref->transform->position = glm::vec3(0.0f, 0.0f, 5.0f);
    // Mesh cube = MeshLoader::LoadMesh("Bench.obj")["Cylinder"];
    // Mesh cube2 = MeshLoader::LoadMesh("Bench.obj")["Cylinder.001"];
    // test2.AddComponent<MeshFilter>(cube2);
    // test2.AddComponent<MeshRenderer>();
    // test.AddComponent<MeshFilter>(cube);
    // MeshRenderer* rf = test.AddComponent<MeshRenderer>();
    // rf->material.texture = Texture::loadTexture("house.png");

    idk.AddComponent<MeshFilter>(MeshLoader::LoadMesh("Cube.obj")["Cube"]);
    MeshRenderer* ref2 = idk.AddComponent<MeshRenderer>();
    
    ref2->material.texture = Texture::loadTexture("blank.png");
    ref2->material.baseColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f);
    

    // Light* lightRef = light.AddComponent<Light>();
    // light.AddComponent<ExampleStandaloneComponent>();
    // light.AddComponent<MeshFilter>(MeshLoader::LoadMesh("Sphere.obj")["Sphere"]);
    // light.AddComponent<MeshRenderer>();
    // lightRef->cutoff = 5;

    // JSONValue json = JSON::Parse(FileProcessor::Read("resources/other/example.json"));

    // int idk2[] = {1, 2, 0, 3, 5};

    // std::vector<uint8_t> bytes = FileProcessor::ReadBytes("resources/models/Cube.glb");
    // auto _text = FileProcessor::ProcessBytes<char>(std::vector<uint8_t>(bytes.begin(),bytes.begin() + 4));
    // for (int i = 0; i < 100; i++) {
    //     Debug::Log(std::string(_text.begin(), _text.end()));
    //     Debug::Warn((std::vector<int>){1, 2, 0, 3, 5});
    //     Debug::Error("Woah");
    // }

    // test.transform.AddChild(&idk.transform);

    

}

glm::vec3 MaxBound(BoundingBox& A, BoundingBox& B)
{
    return { glm::max(A.upperBound.x, B.upperBound.x), glm::max(A.upperBound.y, B.upperBound.y), glm::max(A.upperBound.z, B.upperBound.z) };
}

glm::vec3 MinBound(BoundingBox& A, BoundingBox& B)
{
    return { glm::min(A.lowerBound.x, B.lowerBound.x), glm::min(A.lowerBound.y, B.lowerBound.y), glm::min(A.lowerBound.z, B.lowerBound.z) };
}

BoundingBox Union(BoundingBox& A, BoundingBox& B)
{
    BoundingBox C = BoundingBox();
    C.upperBound = MaxBound(A, B);
    C.lowerBound = MinBound(A, B);
    return C;
}

float SurfaceArea(BoundingBox& box)
{
    glm::vec3 d = box.upperBound - box.lowerBound;
    return 2 * (glm::abs(d.x * d.y) + glm::abs(d.x * d.z) + glm::abs(d.z * d.y));
}