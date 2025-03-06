#include "ptc_camera.h"
#include "ptc_object.h"

Object camera("camera"), text("text");

void game_main()
{
    Camera* camComp = camera.AddComponent<Camera>(false, true);
    camComp->backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
}