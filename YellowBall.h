#pragma once
#include <d3dx9.h>
#include <vector>
#include <memory>
#include <array>

#include "Renderable.h"
#include "CPR_Framework.h"
#include "NavMeshItem.h"

class YellowBallAI;

class YellowBall : public Renderable {
public:
    YellowBall(std::shared_ptr<NavMeshItem> item);
    YellowBall(std::shared_ptr<NavMeshItem> item, float height);
    ~YellowBall();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;
    D3DXVECTOR3 GetSpeed() const;
    std::shared_ptr<YellowBall> seeAnyLeader() const;
    std::shared_ptr<NavMeshItem> GetCurrentNavMeshItem() const;
	bool isBallLeader() const { return isLeader; };
    void SetLeader();
    void UnsetLeader();
	bool checkTargetLeader(std::shared_ptr<YellowBall> ball) const;
	void UnsetTargetLeader();
	bool aquireSlot(int index, YellowBall* ball);

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 upDir{ 0.0f, 1.0f, 0.0f };

    const float visibilityDistance = 100.0f;

    const float weight = 0.1f;
    const float ballSize = 0.2f;

	std::array<YellowBall*, 4> slotAquired{ nullptr, nullptr, nullptr, nullptr };

    const D3DXVECTOR3 scale;
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR4 color{1.0f, 1.0f, 0.0f, 1.0f};

    std::shared_ptr<YellowBallAI> ai;
    bool isLeader{ false };

    friend class YellowBallAI;
};

using YellowBallVectorType = std::vector<std::shared_ptr<YellowBall>>;