#pragma once
#include "Renderable.h"
#include "EngineTypes.h"
#include <d3dx9.h>
#include <vector>
#include <memory>


class NavMesh : public Renderable {
public:
    NavMesh(int _levelSizeX, int _levelSizeZ);
    ~NavMesh();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

    std::shared_ptr<NavMeshItem> getRandom() const;

    NavMeshItemsVector FindNeighbors(int id, float x, float z) const;
protected:
    int levelSizeX;
    int levelSizeZ;

    NavMeshItemsVector navMeshItems;
};

