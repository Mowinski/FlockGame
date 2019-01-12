#include "LevelGround.h"
#include "Game.h"



LevelGround::LevelGround()
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
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ mapSize, 0.10f, mapSize },
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
