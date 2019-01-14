#include "Game.h"
#include "LevelGround.h"
#include "YellowBall.h"
#include "Player.h"
#include "GameHUD.h"
#include <cstdlib>
#include <ctime>


Game* Game::game = nullptr;

Game * Game::GetInstance()
{
    if (game == nullptr) {
        game = new Game{};
    }
    return game;
}

bool Game::Init(std::string filename)
{
    levelFilename = filename;
    if (!RetrieveGraphicDevice()) { return false; }
    loadingScreen = std::make_shared<LoadingScreen>();
    if (!loadingScreen->OnInit()) { return false; }
    loader = std::make_shared<Loader>();

    loadingThread = new std::thread(&Game::Loading, this);
    return true;
}

void Game::Update(float deltaTime)
{
    if (isLoading) {
        loadingScreen->OnUpdate(deltaTime);
        return;
    }
    if (loadingThread->joinable()) { loadingThread->join(); }

    timer->updateTime(deltaTime);
    if (!timer->canStartNextFrame()) { return; }

    for (auto actor : actors) {
        actor->OnUpdate(timer->getDeltaTime());
    }
    timer->clearTimeSinceLastRender();
}

void Game::Render()
{
    if (isLoading) {
        loadingScreen->OnRender();
        return;
    }
    for (auto actor : actors) {
        actor->OnRender();
    }
}

bool Game::RetrieveGraphicDevice()
{
    Mesh* mesh = LoadFromFile("resources/meshes/unitbox.x");
    if (mesh == nullptr) { return false; }
    HRESULT result = mesh->m_mesh->GetDevice(&graphicDevice);
    Release(mesh);
    return SUCCEEDED(result);
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
    for (int i = 0; i < 5; i++) {
        auto ball = std::make_shared<YellowBall>(navMesh->getRandom());
        if (!ball->OnInit()) { return false; }
        actors.push_back(ball);
        blackboard->yellowBalls.push_back(ball);
    }
    return true;
}


void Game::Loading()
{
	std::srand(std::time(nullptr));
    const int delayTime = 10;
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PLAYER;
    actors.push_back(std::make_shared<Player>());
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::GAME_HUD;
    actors.push_back(std::make_shared<GameHUD>());
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    for (auto actor : actors) {
        if (!actor->OnInit()) { return; }
    }
    loadingScreen->state = LoadingState::PREPARE_LEVEL;
    if (!PrepareLevel(levelFilename)) { return; }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_NAV_MESH;
    if (!PrepareNavMesh()) { return; }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_BLACKBOARD;
    blackboard = std::make_unique<Blackboard>(navMesh);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_YELLOW_BALL;
    if (!PrepareInitialYellowBalls()) { return; }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    timer = std::make_shared<Timer>();
    loadingScreen->state = LoadingState::GAME_IS_READY;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    isLoading = false;
}

Game::Game()
{
}

Game::~Game()
{
    if (loadingThread->joinable()) { loadingThread->join(); }
    graphicDevice->Release();
}