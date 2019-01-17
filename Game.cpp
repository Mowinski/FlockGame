#include "Game.h"
#include "YellowBall.h"

#include <cstdlib>
#include <ctime>


Game* Game::game = nullptr;

Game * Game::getInstance()
{
    if (game == nullptr) {
        game = new Game{};
    }
    return game;
}

bool Game::init(std::string filename)
{
    levelFilename = filename;
    if (!retrieveGraphicDevice()) { return false; }
    loadingScreen = std::make_shared<LoadingScreen>();
    if (!loadingScreen->OnInit()) { return false; }
    loader = std::make_shared<Loader>();

    loadingThread = new std::thread(&Game::loading, this);
    return true;
}

void Game::update(float deltaTime)
{
    if (isLoading) {
        loadingScreen->OnUpdate(deltaTime);
        return;
    }
    if (loadingThread && loadingThread->joinable()) { loadingThread->join(); }

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
    blackboard->clearDeadRedBalls();
    timer->clearTimeSinceLastRender();
}

void Game::render()
{
    if (isLoading) {
        loadingScreen->OnRender();
        return;
    }
    player->OnRender();
    hud->OnRender();
    levelGround->OnRender();
    city->OnRender();
    for (auto ball : blackboard->yellowBalls) {
        ball->OnRender();
    }
    for (auto ball : blackboard->redBalls) {
        ball->OnRender();
    }
}

bool Game::retrieveGraphicDevice()
{
    Mesh* mesh = LoadFromFile("resources/meshes/unitbox.x");
    if (mesh == nullptr) { return false; }
    HRESULT result = mesh->m_mesh->GetDevice(&graphicDevice);
    Release(mesh);
    return SUCCEEDED(result);
}

bool Game::prepareLevel(const std::string& filename)
{
    city = std::make_shared<City>(filename);
    if (!city->OnInit()) { return false; }

    levelGround = std::make_shared<LevelGround>(city->getMapWidth(), city->getMapHeight());
    if (!levelGround->OnInit()) { return false; }
    return true;
}

void Game::prepareNavMesh()
{
    navMesh = std::make_shared<NavMesh>(city->getMapWidth(), city->getMapHeight());
	navMesh->onInit();
}

bool Game::prepareInitialYellowBalls()
{
    for (int i = 0; i < blackboard->startingBallsCount; i++) {
        auto ball = std::make_shared<YellowBall>(navMesh->getRandom());
        if (!ball->OnInit()) { return false; }
        blackboard->yellowBalls.push_back(ball);
    }
    blackboard->nominateLeaders();
    return true;
}


void Game::loading()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
	// Loading time has been extended specially to be able to admire the loading screen
    const int delayTime = 400;
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
    if (!prepareLevel(levelFilename)) { return; }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_NAV_MESH;
	prepareNavMesh();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_BLACKBOARD;
    blackboard = std::make_unique<Blackboard>();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    loadingScreen->state = LoadingState::PREPARE_YELLOW_BALL;
    if (!prepareInitialYellowBalls()) { return; }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

    timer = std::make_unique<Timer>();
    loadingScreen->state = LoadingState::GAME_IS_READY;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    isLoading = false;
}

Game::~Game()
{
    if (loadingThread->joinable()) { loadingThread->join(); }
    graphicDevice->Release();
}