#include "LevelGround.h"




LevelGround::LevelGround()
{

}


LevelGround::~LevelGround()
{
    Release(ground);
}

void LevelGround::OnUpdate(float deltaTime)
{
}

void LevelGround::OnRender()
{
    Render(ground,
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ mapSize, 0.10f, mapSize },
           D3DXVECTOR4{ 0.0f, 1.0f, 0.0f, 1.0f });
}

bool LevelGround::OnInit()
{
    ground = LoadFromFile("resources/meshes/unitbox.x");
    if (ground == nullptr) {
        MessageBox(NULL, "Error loading ground", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}
