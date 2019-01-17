#pragma once
#include <d3dx9.h>

class Renderable {
public:
    Renderable() = default;
    virtual ~Renderable() = default;
    virtual void onRender() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual bool onInit() = 0;
};

