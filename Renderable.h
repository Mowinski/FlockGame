#pragma once
class Renderable {
public:
    Renderable();
    virtual ~Renderable();
    virtual void OnRender() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual bool OnInit() = 0;
};

