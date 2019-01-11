#include "Building.h"

Building::Building(D3DXVECTOR3 _position, int _height) : position{ _position }, height { _height }
{
    position.y = static_cast<float>(height / 2.0f);
}


Building::~Building()
{
    if(buildingMesh != nullptr) { Release(buildingMesh); }
}

void Building::OnUpdate(float deltaTime)
{
}

void Building::OnRender()
{
    Render(buildingMesh,
           position,
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ buildingSize, static_cast<float>(height), buildingSize },
           D3DXVECTOR4{ 1.0f, 0.0f, 0.0f, 1.0f });

}

bool Building::OnInit()
{
    buildingMesh = LoadFromFile("resources/meshes/unitbox.x");
    if (buildingMesh == nullptr) {
        MessageBox(NULL, "Error loading building", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

