#pragma once
#include <d3dx9math.h>

class AABBCollisionBox {
public:
    AABBCollisionBox(const D3DXVECTOR3& minPoint, const D3DXVECTOR3& maxPoint);
    ~AABBCollisionBox() = default;

    bool isCollide(const D3DXVECTOR3& point) const;
    bool isCollide(const AABBCollisionBox& box) const;

protected:
    const D3DXVECTOR3 vecMin;
	const D3DXVECTOR3 vecMax;
};

