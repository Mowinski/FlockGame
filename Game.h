#pragma once
#include "Blackboard.h"
#include "City.h"
#include "GameHUD.h"
#include "LevelGround.h"
#include "Loader.h"
#include "LoadingScreen.h"
#include "NavMesh.h"
#include "Player.h"
#include "Timer.h"
#include <memory>
#include <thread>

class Game {
public:
    ~Game();

    static Game* getInstance();

    bool init(std::string filename);
    void update(float deltaTime);
    void render();

    LPDIRECT3DDEVICE9 graphicDevice;
    std::shared_ptr<City> city{ nullptr };
	std::shared_ptr<Loader> loader{ nullptr };
	std::shared_ptr<NavMesh> navMesh{ nullptr };
	std::unique_ptr<Blackboard> blackboard{ nullptr };
	std::unique_ptr<Timer> timer{ nullptr };

private:
    Game() = default;
    bool retrieveGraphicDevice();
    bool prepareLevel(const std::string &filename);
    void prepareNavMesh();
    bool prepareInitialYellowBalls();
    void loading();

    static Game* game;

    bool isLoading{ true };
    std::string levelFilename;
    std::shared_ptr<Player> player{ nullptr };
    std::shared_ptr<GameHUD> hud{ nullptr };
    std::shared_ptr<LevelGround> levelGround{ nullptr };
    std::shared_ptr<LoadingScreen> loadingScreen{ nullptr };
	std::thread* loadingThread{ nullptr };
};
