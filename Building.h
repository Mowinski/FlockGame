#pragma once
#include "Renderable.h"
#include "CPR_Framework.h"
#include "AABBCollisionBox.h"
#include <array>

enum BuildingWall {
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3,
};

class Building : public Renderable {
public:
    Building(D3DXVECTOR3 _position, int _height);
    ~Building();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;
    float getDistanceBetweenCenterAndPoint(D3DXVECTOR3 point);
    bool isCollide(D3DXVECTOR3 point) const;
    bool isCollide(const AABBCollisionBox & box) const;
    D3DXVECTOR3 GetPosition() const override;
	D3DXVECTOR3 GetNormalAtPoint(const D3DXVECTOR3& point) const;

protected:
	const int height;
	const float buildingSize = 4.0f;
    const D3DXVECTOR3 position;
	const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 scale;
	const D3DXVECTOR4 color{ 0.0f, 0.0f, 1.0f, 1.0f };
    
	std::array<D3DXVECTOR3, 4> wallCenter;
	std::array<D3DXVECTOR3, 4> normals;
    const D3DXVECTOR3 minPoint;
    const D3DXVECTOR3 maxPoint;
};

