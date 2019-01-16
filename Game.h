#pragma once
#include <memory>
#include <vector>
#include <thread>

#include "Renderable.h"
#include "City.h"
#include "Timer.h"
#include "Loader.h"
#include "NavMesh.h"
#include "LoadingScreen.h"
#include "Blackboard.h"
#include "Player.h"
#include "GameHUD.h"
#include "LevelGround.h"

class Game {
public:
    ~Game();

    static Game* GetInstance();

    bool Init(std::string filename);
    void Update(float deltaTime);
    void Render();

    LPDIRECT3DDEVICE9 graphicDevice;
    std::shared_ptr<City> city{ nullptr };
    std::shared_ptr<Loader> loader;
    std::shared_ptr<NavMesh> navMesh;
    std::unique_ptr<Blackboard> blackboard;

private:
    Game();
    bool RetrieveGraphicDevice();
    bool PrepareLevel(const std::string &filename);
    bool PrepareNavMesh();
    bool PrepareInitialYellowBalls();

    void Loading();

    static Game* game;

    int ballsCount{ 35 };
    bool isLoading{ true };
    std::string levelFilename;
    std::shared_ptr<Player> player{ nullptr };
    std::shared_ptr<GameHUD> hud{ nullptr };
    std::shared_ptr<Timer> timer{ nullptr };
    std::shared_ptr<LevelGround> levelGround{ nullptr };
    std::shared_ptr<LoadingScreen> loadingScreen{ nullptr };
    std::thread* loadingThread;
};
