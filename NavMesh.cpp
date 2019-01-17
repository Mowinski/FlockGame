#include "NavMesh.h"
#include "Game.h"

NavMesh::NavMesh(float _levelSizeX, float _levelSizeZ) : levelSizeX{_levelSizeX}, levelSizeZ(_levelSizeZ)
{
}

void NavMesh::onInit()
{
	generateNavMeshItems();
	generateConnectionBetweenNavMeshItem();
}

std::shared_ptr<NavMeshItem> NavMesh::getRandom() const
{
    auto randIt = navMeshItems.begin();
    std::advance(randIt, std::rand() % navMeshItems.size());
    return *randIt;
}

NavMeshItemsVectorType NavMesh::findNeighbors(int id, float x, float z) const
{
    NavMeshItemsVectorType items{};
    for (auto item : navMeshItems) {
        if (item->id == id) { continue; }
        D3DXVECTOR2 distanceVec{ item->GetPosition().x - x, item->GetPosition().z - z };
        if (D3DXVec2Length(&distanceVec) < maxDistanceBetweenNeighbors) { items.push_back(item); }
    }

    return items;
}

void NavMesh::generateNavMeshItem(int id, const D3DXVECTOR3& position)
{
	auto item = std::make_shared<NavMeshItem>(id, position.x, position.z);
	navMeshItems.push_back(item);
}

void NavMesh::generateConnectionBetweenNavMeshItem()
{

	for (auto navMeshItem : navMeshItems) {
		NavMeshItemsVectorType neighbors = findNeighbors(navMeshItem->id, navMeshItem->GetPosition().x, navMeshItem->GetPosition().z);
		navMeshItem->AddNeighbors(neighbors);
	}
}

void NavMesh::generateNavMeshItems()
{
	const float halfOfLevelSizeX = levelSizeX * 0.5f;
	const float halfOfLevelSizeZ = levelSizeZ * 0.5f;

	int id = 0;
	for (float x = -halfOfLevelSizeX; x < halfOfLevelSizeX; ++x) {
		for (float z = -halfOfLevelSizeZ; z < halfOfLevelSizeZ; ++z) {
			D3DXVECTOR3 navMeshPosition{ x, 0.5f, z };
			if (Game::getInstance()->city->isCollideWithAnyBuilding(navMeshPosition, 8.0f)) { continue; }
			generateNavMeshItem(id++, navMeshPosition);
		}
	}
}
