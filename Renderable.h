#pragma once
#include <d3dx9.h>

class Renderable {
public:
    Renderable();
    virtual ~Renderable();
    virtual void OnRender() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual bool OnInit() = 0;
    virtual D3DXVECTOR3 GetPosition() const = 0;
};

