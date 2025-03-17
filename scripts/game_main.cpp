#include "ptc_camera.h"
#include "ptc_object.h"
#include "standalone/game_manager.cpp"
#include <cstdlib>
#include <ctime>

Object camera("camera"), gameManager("gameManager");

void game_main()
{
    srand(time(0));
    camera.AddComponent<Camera>();
    gameManager.AddComponent<GameManager>(true);
}