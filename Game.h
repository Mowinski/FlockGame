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

class Game {
public:
    ~Game();

    static Game* GetInstance();

    bool Init(std::string filename);
    void Update(float deltaTime);
    void Render();

    LPDIRECT3DDEVICE9 graphicDevice;
    std::shared_ptr<City> city;
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

    bool isLoading{ true };
    std::string levelFilename;
    std::vector<std::shared_ptr<Renderable>> actors;
    std::shared_ptr<Timer> timer;
    std::shared_ptr<LoadingScreen> loadingScreen;
    std::thread* loadingThread;
};
