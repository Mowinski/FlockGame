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

void NavMesh::generateConnectionBetweenNavMeshItem()
{

	for (auto navMeshItem : navMeshItems) {
		NavMeshItemsVectorType neighbors = findNeighbors(navMeshItem->id, navMeshItem->position.x, navMeshItem->position.z);
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
			if (Game::GetInstance()->city->isCollideWithAnyBuilding(navMeshPosition, 8.0f)) { continue; }
			generateNavMeshItem(id++, navMeshPosition);
		}
	}
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
        D3DXVECTOR2 distanceVec{ item->position.x - x, item->position.z - z };
        if (D3DXVec2Length(&distanceVec) < 1.5) { items.push_back(item); }
    }

    return items;
}

void NavMesh::generateNavMeshItem(int id, const D3DXVECTOR3& position)
{
	auto item = std::make_shared<NavMeshItem>(id, position.x, position.z);
	item->OnInit();
	navMeshItems.push_back(item);
}


