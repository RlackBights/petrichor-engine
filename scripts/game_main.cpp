#include "ptc_camera.h"
#include "ptc_object.h"
#include "standalone/game_manager.cpp"

Object camera("camera"), gameManager("gameManager");

void game_main()
{
    camera.AddComponent<Camera>();
    gameManager.AddComponent<GameManager>();
}