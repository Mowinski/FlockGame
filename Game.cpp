#include "Game.h"
#include "LevelGround.h"
#include "YellowBall.h"
#include "Player.h"
#include "GameHUD.h"

Game* Game::game = nullptr;

Game::Game()
{
}

bool Game::RetrieveGraphicDevice()
{
    Mesh* mesh = LoadFromFile("resources/meshes/unitbox.x");
    if (mesh == nullptr) { return false; }
    HRESULT result = mesh->m_mesh->GetDevice(&graphicDevice);
    Release(mesh);
    return SUCCEEDED(result);
}


Game::~Game()
{
    graphicDevice->Release();
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
    timer->updateTime(deltaTime);
    if (!timer->canStartNextFrame()) { return; }

    for (auto actor : actors) {
        actor->OnUpdate(timer->getDeltaTime());
    }
    timer->clearTimeSinceLastRender();
}

void Game::Render()
{
    for (auto actor : actors) {
        actor->OnRender();
    }
    navMesh->OnRender();
}


bool Game::Init(std::string filename)
{
    if (!RetrieveGraphicDevice()) { return false; }
    loader = std::make_shared<Loader>();
    actors.push_back(std::make_shared<Player>());
    actors.push_back(std::make_shared<GameHUD>());

    for (auto actor : actors) {
        if (!actor->OnInit()) { return false; }
    }
    if (!PrepareLevel(filename)) { return false; }
    if (!PrepareNavMesh()) { return false; }
    if (!PrepareInitialYellowBalls()) { return false; }

    timer = std::make_shared<Timer>();

    return true;
}

bool Game::PrepareLevel(const std::string& filename)
{
    city = std::make_shared<City>(filename);
    if (!city->OnInit()) { return false; }
    actors.push_back(std::make_shared<LevelGround>(city->getMapWidth(), city->getMapHeight()));
    actors.push_back(city);
    return true;
}

bool Game::PrepareNavMesh()
{
    navMesh = std::make_shared<NavMesh>(city->getMapWidth(), city->getMapHeight());
    if (!navMesh->OnInit()) { return false; }
    return true;
}

bool Game::PrepareInitialYellowBalls()
{
    for (int i = 0; i < 2; i++) {
        auto ball = std::make_shared<YellowBall>(navMesh->getRandom());
        if (!ball->OnInit()) { return false; }
        actors.push_back(ball);
    }
    return true;
}