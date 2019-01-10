#include "Game.h"



Game::Game()
{
}


Game::~Game()
{
}

void Game::Update(float deltaTime)
{
}

void Game::Render()
{
	levelGround->OnRender();
}


bool Game::Init(std::string filename)
{
	LookAt(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	levelGround = std::make_shared<LevelGround>(LevelGround(filename));
	return levelGround->LoadCityFile();
}
