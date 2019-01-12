#pragma once
#include <d3dx9.h>

#include "Renderable.h"
#include "CPR_Framework.h"
#include "YellowBallAI.h"

class YellowBall : public Renderable {
public:
    YellowBall(std::shared_ptr<NavMeshItem> _item);
    ~YellowBall();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };

    const float weight = 0.1f;
    const float ballSize = 0.2f;
    const D3DXVECTOR3 scale;
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR4 color{1.0f, 1.0f, 0.0f, 1.0f};

    std::shared_ptr<YellowBallAI> ai;
};

