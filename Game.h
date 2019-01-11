#pragma once
#include <memory>
#include <vector>

#include "Renderable.h"
#include "LevelGround.h"
#include "Player.h"
#include "Building.h"
#include "City.h"

class Game {
public:
    ~Game();

    static Game* GetInstance();

    void Update(float deltaTime);
    void Render();
    bool Init(std::string filename);

private:
    Game();
    std::vector<std::shared_ptr<Renderable>> actors;

    static Game* game;
};

