#pragma once
#include "Renderable.h"
#include "YellowBall.h"

#include <vector>
#include <memory>

class RedBallAI;

class RedBall : public Renderable {
public:
    RedBall(D3DXVECTOR3 _position, D3DXVECTOR3 lookDir);
    ~RedBall();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;
	D3DXVECTOR3 FindNormal(const D3DXVECTOR3& speedNorm) const;
protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 lookDir;
	D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };

	const float weight = 0.1f;
	const float ballSize = 0.2f;

	const D3DXVECTOR3 gravity{ 0.0f, -0.9f, 0.0f };
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR3 scale{ ballSize, ballSize, ballSize };
    const D3DXVECTOR4 color{ 1.0f, 0.0f, 0.0f, 1.0f };

	std::shared_ptr<RedBallAI> ai;

	friend class RedBallAI;
};

using RedBallVectorType = std::vector<std::shared_ptr<RedBall>>;