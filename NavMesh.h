#pragma once
#include "EngineTypes.h"

#include <d3dx9math.h>
#include <memory>


class NavMesh {
public:
    NavMesh(float _levelSizeX, float _levelSizeZ);
    ~NavMesh() = default;

    void onInit();

    std::shared_ptr<NavMeshItem> getRandom() const;

    NavMeshItemsVectorType navMeshItems;
protected:
    float levelSizeX;
    float levelSizeZ;
	const float maxDistanceBetweenNeighbors{ 1.5f };

	NavMeshItemsVectorType findNeighbors(int id, float x, float z) const;
	void generateNavMeshItem(int id, const D3DXVECTOR3& position) ;
	void generateConnectionBetweenNavMeshItem();
	void generateNavMeshItems();
};
