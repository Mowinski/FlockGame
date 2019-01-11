#include "Game.h"


Game* Game::game = nullptr;

Game::Game()
{
}


Game::~Game()
{
}

Game * Game::GetInstance()
{
    if (game == nullptr) {
        game = new Game{};
    }
    return game;
}

void Game::Update(float deltaTime)
{
    for (auto actor : actors) {
        actor->OnUpdate(deltaTime);
    }
}

void Game::Render()
{
    for (auto actor : actors) {
        actor->OnRender();
    }
}


bool Game::Init(std::string filename)
{
    actors.push_back(std::make_shared<Player>());
    actors.push_back(std::make_shared<LevelGround>());
    actors.push_back(std::make_shared<City>(filename));

    for (auto actor : actors) {
        if(!actor->OnInit()) { return false; }
    }
    return true;
}
