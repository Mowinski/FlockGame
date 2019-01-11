#pragma once
#include <vector>
#include <string>

#include "Renderable.h"
#include "CPR_Framework.h"

class LevelGround : public Renderable {
public:
    LevelGround();
    ~LevelGround();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;

private:
    Mesh* ground{ nullptr };
    const float mapSize{ 100.0f };
};

