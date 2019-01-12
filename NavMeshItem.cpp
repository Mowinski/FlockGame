#include "NavMeshItem.h"
#include "Game.h"
#include  <random>
#include  <iterator>


struct line_vertex {
    float x, y, z;
};


NavMeshItem::NavMeshItem(int _id, float x, float z) : id(_id), position{ x, 0.0f, z }, collisionBox{ D3DXVECTOR3{x - 0.5f, 0.0f, z - 0.5f}, D3DXVECTOR3{x + 0.5f, 0.0f, z + 0.5f} }
{
}


NavMeshItem::~NavMeshItem()
{
}

void NavMeshItem::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void NavMeshItem::OnUpdate(float deltaTime)
{
}

bool NavMeshItem::OnInit()
{
    D3DXCreateLine(Game::GetInstance()->graphicDevice, &line);
    return true;
}

D3DXVECTOR3 NavMeshItem::GetPosition() const
{
    return position;
}

void NavMeshItem::AddNeighbors(const NavMeshItemsVector & items)
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
