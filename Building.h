#pragma once
#include "AABBCollisionBox.h"
#include "Renderable.h"
#include <array>

class Building : public Renderable {
public:
    Building(const D3DXVECTOR3& _position, int _height);
    ~Building() = default;

    void onUpdate(float deltaTime) override;
    void onRender() override;
    bool onInit() override;
    float getDistanceBetweenCenterAndPoint(const D3DXVECTOR3& point);
    bool isCollide(const D3DXVECTOR3& point) const;
    bool isCollide(const AABBCollisionBox & box) const;
	D3DXVECTOR3 getNormalAtPoint(const D3DXVECTOR3& point) const;

protected:
	const int height;
	const float buildingSize = 4.0f;
    const D3DXVECTOR3 position;
	const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 scale;
	const D3DXVECTOR4 color{ 0.0f, 0.0f, 1.0f, 1.0f };
    
	std::array<D3DXVECTOR3, 4> wallCenter{};
	std::array<D3DXVECTOR3, 4> normals{};
	const AABBCollisionBox collisionBox;
};
