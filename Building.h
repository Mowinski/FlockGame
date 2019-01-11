#pragma once
#include "Renderable.h"
#include "CPR_Framework.h"

class Building : public Renderable {
public:
    Building(D3DXVECTOR3 _position, int _height);
    ~Building();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;

protected:
    Mesh* buildingMesh{ nullptr };

    D3DXVECTOR3 position;
    int height;
    const float buildingSize = 4.0f;
};

