#pragma once
#include <d3dx9.h>

class AABBCollisionBox {
public:
    AABBCollisionBox(D3DXVECTOR3 p0, D3DXVECTOR3 p1);
    ~AABBCollisionBox();

    bool isCollide(const D3DXVECTOR3& point) const;
    bool isCollide(const AABBCollisionBox& box) const;

protected:
    D3DXVECTOR3 vecMin;
    D3DXVECTOR3 vecMax;
};

