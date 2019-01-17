#include "Building.h"

#include "CPR_Framework.h"
#include "Game.h"


enum BuildingWall {
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3,
};


Building::Building(const D3DXVECTOR3& _position, int _height) :
    position{ _position.x, _height / 2.0f, _position.z },
    height { _height },
    scale{ buildingSize, static_cast<float>(height), buildingSize },
	collisionBox{ D3DXVECTOR3{ _position.x - 0.5f * buildingSize - 0.15f, 0.0f, _position.z - 0.5f * buildingSize - 0.15f },
	              D3DXVECTOR3{ _position.x + 0.5f * buildingSize + 0.15f, static_cast<float>(_height), _position.z + 0.5f * buildingSize + 0.15f } }
{
}

void Building::OnUpdate(float deltaTime)
{
}

void Building::OnRender()
{
    Render(Game::getInstance()->loader->GetMesh("unitbox"), position, rotation, scale, color);
}

bool Building::OnInit()
{
	const float halfSize = buildingSize / 2.0f;
	const float halfHeight = height / 2.0f;

	wallCenter[BuildingWall::NORTH] = D3DXVECTOR3{ position.x + halfSize, halfHeight, position.z };
	wallCenter[BuildingWall::SOUTH] = D3DXVECTOR3{ position.x - halfSize, halfHeight, position.z };
	wallCenter[BuildingWall::EAST] = D3DXVECTOR3{ position.x, halfHeight, position.z - halfSize };
	wallCenter[BuildingWall::WEST] = D3DXVECTOR3{ position.x, halfHeight, position.z + halfSize };

	normals[BuildingWall::NORTH] = D3DXVECTOR3{ 1.0f, 0.0f, 0.0f };
	normals[BuildingWall::SOUTH] = D3DXVECTOR3{ -1.0f, 0.0f, 0.0f };
	normals[BuildingWall::EAST] = D3DXVECTOR3{ 0.0f, 0.0f, -1.0f };
	normals[BuildingWall::WEST] = D3DXVECTOR3{ 0.0f, 0.0f, 1.0f };
    return true;
}

float Building::getDistanceBetweenCenterAndPoint(const D3DXVECTOR3& point)
{
	const float diffX = position.x - point.x;
	const float diffZ = position.z - point.z;
	return std::sqrt(diffX * diffX + diffZ * diffZ);
}

bool Building::isCollide(const D3DXVECTOR3& point) const
{
    return collisionBox.isCollide(point);
}

bool Building::isCollide(const AABBCollisionBox& box) const
{
    return collisionBox.isCollide(box);
}

D3DXVECTOR3 Building::GetPosition() const
{
    return position;
}

D3DXVECTOR3 Building::getNormalAtPoint(const D3DXVECTOR3 & point) const
{
	float distance = (std::numeric_limits<float>::max)();
	short int index = -1;
	for (short int i = 0; i < 4; ++i) {
		float tmpDistance = D3DXVec3Length(&(wallCenter[i] - point));
		if (tmpDistance < distance) {
			index = i;
			distance = tmpDistance;
		}
	}
	return normals[index];
}
