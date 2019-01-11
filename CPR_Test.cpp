#include "CPR_Framework.h"
#include "Game.h"

Game*   g_game = nullptr;

//----------------------------------------------------------------------------
void OnInit()
{
    if (g_game != nullptr) { return; }
    g_game = Game::GetInstance();
    if (!g_game->Init("resources/city.txt")) {
        MessageBox(NULL, "Cannot initialize game", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
}

//----------------------------------------------------------------------------
void OnShutdown()
{
    delete g_game;
}

//----------------------------------------------------------------------------
void OnUpdate(float deltaTime)
{
    g_game->Update(deltaTime);
}

//----------------------------------------------------------------------------
void OnRender()
{
    g_game->Render();
}

CPR_MAIN(OnInit, OnShutdown, OnUpdate, OnRender)
