#pragma once
#include <memory>
#include <vector>

#include "Renderable.h"
#include "City.h"
#include "Timer.h"
#include "Loader.h"
#include "NavMesh.h"

class Game {
public:
    ~Game();

    static Game* GetInstance();

    void Update(float deltaTime);
    void Render();
    bool Init(std::string filename);




    LPDIRECT3DDEVICE9 graphicDevice;
    std::shared_ptr<City> city;
    std::shared_ptr<Loader> loader;
    std::shared_ptr<NavMesh> navMesh;
private:
    Game();
    bool RetrieveGraphicDevice();
    bool PrepareLevel(const std::string &filename);
    bool PrepareNavMesh();
    bool PrepareInitialYellowBalls();

    std::vector<std::shared_ptr<Renderable>> actors;
    std::shared_ptr<Timer> timer;


    static Game* game;
};

