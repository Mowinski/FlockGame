#pragma once
#include "Building.h"
#include "Renderable.h"
#include <d3dx9math.h>
#include <memory>
#include <vector>

class RedBallAI;

class RedBall : public Renderable {
public:
    RedBall(const D3DXVECTOR3& _position, const D3DXVECTOR3& _lookDir);
    ~RedBall() = default;

    void onRender() override;
    void onUpdate(float deltaTime) override;
    bool onInit() override;

	D3DXVECTOR3 getPosition() const { return position; };
	bool isDead() const { return position.y <= groundLevel; };

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
	std::shared_ptr<RedBallAI> ai{ nullptr };

    const float weight = 0.1f;
    const float ballDiameter = 0.2f;
    const float groundLevel = 0.05f;

    const D3DXVECTOR3 gravity{ 0.0f, -0.9f, 0.0f };
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR3 scale{ ballDiameter, ballDiameter, ballDiameter };
    const D3DXVECTOR4 color{ 1.0f, 0.0f, 0.0f, 1.0f };

	void checkHitWithYellowBall() const;
	D3DXVECTOR3 calculateNewSpeed(float deltaTime) const;
	D3DXVECTOR3 calculateReflectionSpeed(const std::shared_ptr<Building>& building) const;


    friend class RedBallAI;
};

using RedBallVectorType = std::vector<std::shared_ptr<RedBall>>;