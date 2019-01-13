#pragma once
#include "Renderable.h"
#include <vector>
#include <memory>

class RedBall : public Renderable {
public:
    RedBall(D3DXVECTOR3 _position, D3DXVECTOR3 lookDir);
    ~RedBall();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 lookDir;
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR3 scale{ 0.2f, 0.2f, 0.2f };
    const D3DXVECTOR4 color{ 1.0f, 0.0f, 0.0f, 1.0f };

    float energy;
};

using RedBallVectorType = std::vector<std::shared_ptr<RedBall>>;