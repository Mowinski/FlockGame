#pragma once
#include <vector>
#include <string>

#include "Renderable.h"
#include "CPR_Framework.h"

class LevelGround : public Renderable {
public:
    LevelGround(float _mapSizeX, float _mapSizeZ);
    ~LevelGround();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

private:
    const float mapSizeX;
    const float mapSizeZ;
};

