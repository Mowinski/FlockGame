#include "NavMesh.h"
#include "Game.h"

NavMesh::NavMesh(int _levelSizeX, int _levelSizeZ) : levelSizeX{_levelSizeX}, levelSizeZ(_levelSizeZ)
{
}


NavMesh::~NavMesh()
{
}

void NavMesh::OnRender()
{
    for (auto item : navMeshItems) {
        item->OnRender();
    }
}

void NavMesh::OnUpdate(float deltaTime)
{
}

bool NavMesh::OnInit()
{
    const float halfOfLevelSizeX = levelSizeX * 0.5f;
    const float halfOfLevelSizeZ = levelSizeZ * 0.5f;

    int id = 0;
    for (float x = -halfOfLevelSizeX; x < halfOfLevelSizeX; ++x) {
        for (float z = -halfOfLevelSizeZ; z < halfOfLevelSizeZ; ++z) {
            auto item = std::make_shared<NavMeshItem>(id++, x, z);
            if (Game::GetInstance()->city->isCollideWithAnyBuilding(item, 8.0f)) { continue; }
            item->OnInit();
            navMeshItems.push_back(item);
        }
    }

    for (auto navMeshItem : navMeshItems) {
        NavMeshItemsVector neighbors = FindNeighbors(navMeshItem->id, navMeshItem->position.x, navMeshItem->position.z);
        navMeshItem->AddNeighbors(neighbors);
    }
    return true;
}

D3DXVECTOR3 NavMesh::GetPosition() const
{
    return D3DXVECTOR3();
}

std::shared_ptr<NavMeshItem> NavMesh::getRandom() const
{
    auto randIt = navMeshItems.begin();
    std::advance(randIt, std::rand() % navMeshItems.size());
    return *randIt;
}

NavMeshItemsVector NavMesh::FindNeighbors(int id, float x, float z) const
{
    NavMeshItemsVector items{};
    for (auto item : navMeshItems) {
        if (item->id == id) { continue; }
        D3DXVECTOR2 distanceVec{ item->position.x - x, item->position.z - z };
        if (D3DXVec2Length(&distanceVec) < 1.5) { items.push_back(item); }
    }

    return items;
}


