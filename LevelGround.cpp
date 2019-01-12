#include "LevelGround.h"
#include "Game.h"



LevelGround::LevelGround(float _mapSizeX, float _mapSizeZ) : mapSizeX(_mapSizeX), mapSizeZ(_mapSizeZ)
{
}


LevelGround::~LevelGround()
{
}

void LevelGround::OnUpdate(float deltaTime)
{
}

void LevelGround::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitbox"),
           D3DXVECTOR3{ -0.5f, 0.0f, -0.5f },
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ mapSizeX, 0.10f, mapSizeZ },
           D3DXVECTOR4{ 0.0f, 1.0f, 0.0f, 1.0f });
}

bool LevelGround::OnInit()
{
    return true;
}

D3DXVECTOR3 LevelGround::GetPosition() const
{
    return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
