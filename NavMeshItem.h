#pragma once
#include "Renderable.h"
#include "CPR_Framework.h"
#include "EngineTypes.h"
#include "AABBCollisionBox.h"

#include <d3dx9.h>
#include <memory>
#include <vector>



class NavMeshItem : public Renderable {
public:
    NavMeshItem(int _id, float x, float z);
    ~NavMeshItem();

    void OnRender();
    void OnUpdate(float deltaTime);
    bool OnInit();
    D3DXVECTOR3 GetPosition() const override;
    float CalculateDist(float x, float z) const;

    void AddNeighbors(const NavMeshItemsVectorType& items);
    std::shared_ptr<NavMeshItem> GetRandomNeighbor() const;

    const int id;
    const D3DXVECTOR3 position;
    const AABBCollisionBox collisionBox;
    NavMeshItemsVectorType neighbors;

protected:
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR3 scale{ 1.0f, 0.2f, 1.0f };
    const D3DXVECTOR4 color{ 0.0f, 0.0f, 1.0f, 1.0f };

    LPD3DXLINE line;
};

