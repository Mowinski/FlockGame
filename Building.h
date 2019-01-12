#pragma once
#include "Renderable.h"
#include "CPR_Framework.h"
#include "AABBCollisionBox.h"

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
    void changeColor(D3DXVECTOR4 _color);
    D3DXVECTOR3 GetPosition() const override;

protected:
    const D3DXVECTOR3 position;
    D3DXVECTOR4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
    const int height;
    const float buildingSize = 4.0f;

    const D3DXVECTOR3 minPoint;
    const D3DXVECTOR3 maxPoint;
};

