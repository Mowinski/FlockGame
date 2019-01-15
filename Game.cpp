#include "Game.h"
#include "YellowBall.h"

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
    float timeSinceLastFrame = timer->getDeltaTime();

    player->OnUpdate(timeSinceLastFrame);
    hud->OnUpdate(timeSinceLastFrame);
    levelGround->OnUpdate(timeSinceLastFrame);
    city->OnUpdate(timeSinceLastFrame);
    for (auto ball : blackboard->yellowBalls) {
        ball->OnUpdate(timeSinceLastFrame);
    }
    for (auto ball : blackboard->redBalls) {
        ball->OnUpdate(timeSinceLastFrame);
    }
    blackboard->clearRedBalls();
    timer->clearTimeSinceLastRender();
}

void Game::Render()
{
    if (isLoading) {
        loadingScreen->OnRender();
        return;
    }
    player->OnRender();
    hud->OnRender();
    levelGround->OnRender();
    city->OnRender();
    //navMesh->OnRender();
    for (auto ball : blackboard->yellowBalls) {
        ball->OnRender();
    }
    for (auto ball : blackboard->redBalls) {
        ball->OnRender();
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
    levelGround = std::make_shared<LevelGround>(city->getMapWidth(), city->getMapHeight());
    if (!levelGround->OnInit()) { return false; }
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
    for (int i = 0; i < ballsCount; i++) {
        auto ball = std::make_shared<YellowBall>(navMesh->getRandom());
        if (!ball->OnInit()) { return false; }
        blackboard->yellowBalls.push_back(ball);
    }
    blackboard->nominateLeaders();
    return true;
}


void Game::Loading()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    const int delayTime = 10;
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PLAYER;
    player = std::make_shared<Player>();
    player->OnInit();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::GAME_HUD;
    hud = std::make_shared<GameHUD>();
    hud->OnInit();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

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