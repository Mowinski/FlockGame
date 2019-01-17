#include "NavMeshItem.h"
#include  <random>


NavMeshItem::NavMeshItem(int _id, float x, float z) :
	id(_id),
	position{ x, 0.0f, z },
	collisionBox{ D3DXVECTOR3{x - 0.5f, 0.0f, z - 0.5f}, D3DXVECTOR3{x + 0.5f, 0.0f, z + 0.5f} }
{
}

float NavMeshItem::CalculateDistance(float x, float z) const
{
    D3DXVECTOR2 diff(position.x - x, position.z - z);
    return D3DXVec2Length(&diff);
}

void NavMeshItem::AddNeighbors(const NavMeshItemsVectorType & items)
{
    for (auto item : items) {
        neighbors.push_back(item);
    }
}

std::shared_ptr<NavMeshItem> NavMeshItem::GetRandomNeighbor() const
{
    auto randIt = neighbors.begin();
    std::advance(randIt, std::rand() % neighbors.size());
    return *randIt;
}
