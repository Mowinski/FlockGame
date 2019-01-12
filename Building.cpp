#include "Building.h"
#include "Game.h"


Building::Building(D3DXVECTOR3 _position, int _height) :
    position{ _position.x, _height / 2.0f, _position.z },
    height { _height },
    minPoint{ _position.x - 0.5 * buildingSize - 0.15f, -10.0f, _position.z - 0.5 * buildingSize - 0.15f },
    maxPoint{ _position.x + 0.5 * buildingSize + 0.15f, 10.0f, _position.z + 0.5 * buildingSize + 0.15f }
{
}


Building::~Building()
{
}

void Building::OnUpdate(float deltaTime)
{
}

void Building::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitbox"),
           position,
           D3DXVECTOR3{ 0.0f, 0.0f, 0.0f },
           D3DXVECTOR3{ buildingSize, static_cast<float>(height), buildingSize },
           color);

}

bool Building::OnInit()
{
    return true;
}

float Building::getDistanceBetweenCenterAndPoint(D3DXVECTOR3 point)
{
    return D3DXVec3Length(&(position - point));
}

bool Building::isCollide(D3DXVECTOR3 point) const
{
    AABBCollisionBox collisionBox{ minPoint, maxPoint };
    return collisionBox.isCollide(point);
}

bool Building::isCollide(const AABBCollisionBox& box) const
{
    AABBCollisionBox collisionBox{ minPoint, maxPoint };
    return collisionBox.isCollide(box);
}

void Building::changeColor(D3DXVECTOR4 _color)
{
    color = _color;
}

D3DXVECTOR3 Building::GetPosition() const
{
    return position;
}
