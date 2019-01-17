#pragma once
#include "AABBCollisionBox.h"
#include "EngineTypes.h"
#include <d3dx9math.h>
#include <memory>


class NavMeshItem {
public:
    NavMeshItem(int _id, float x, float z);
    ~NavMeshItem() = default;


	D3DXVECTOR3 GetPosition() const { return position; };
    float CalculateDistance(float x, float z) const;

    void AddNeighbors(const NavMeshItemsVectorType& items);
    std::shared_ptr<NavMeshItem> GetRandomNeighbor() const;

    const int id;
    const AABBCollisionBox collisionBox;
    NavMeshItemsVectorType neighbors;

protected:
	const D3DXVECTOR3 position;
};

