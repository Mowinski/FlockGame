#pragma once
#include "NavMeshItem.h"
#include "Renderable.h"
#include "Utils.h"

#include <array>
#include <d3dx9math.h>
#include <limits>
#include <memory>
#include <vector>

class YellowBallAI;

class YellowBall : public Renderable {
public:
	YellowBall(std::shared_ptr<NavMeshItem> item);
	YellowBall(std::shared_ptr<NavMeshItem> item, float height);
	~YellowBall() = default;

	void onRender() override;
	void onUpdate(float deltaTime) override;
	bool onInit() override;

	D3DXVECTOR3 getPosition() const { return position; };
	std::shared_ptr<YellowBall> getNearestLeaderIfAny() const;
	std::shared_ptr<NavMeshItem> getCurrentNavMeshItem() const { return Utils::getNearestNavMeshItem(position); };

	bool isBallLeader() const { return isLeader; };
    void setLeader();
    void unsetLeader();
	bool checkTargetLeader(const std::shared_ptr<YellowBall>& ball) const;
	void unsetTargetLeader();

	bool aquireSlot(int index, YellowBall* ball);

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 upDir{ 0.0f, 1.0f, 0.0f };

    const float visibilityDistance = (std::numeric_limits<float>::max)();
    const float weight = 0.1f;
    const float ballDiameter = 0.2f;

	std::array<YellowBall*, 4> slotAquired{ nullptr, nullptr, nullptr, nullptr };

    const D3DXVECTOR3 scale{ ballDiameter, ballDiameter, ballDiameter };
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    const D3DXVECTOR4 color{ 1.0f, 1.0f, 0.0f, 1.0f };

	std::shared_ptr<YellowBallAI> ai{ nullptr };
    bool isLeader{ false };

    friend class YellowBallAI;
};

using YellowBallVectorType = std::vector<std::shared_ptr<YellowBall>>;