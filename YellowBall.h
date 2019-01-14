#pragma once
#include <d3dx9.h>
#include <vector>
#include <memory>

#include "Renderable.h"
#include "CPR_Framework.h"
#include "NavMeshItem.h"

class YellowBallAI;

class YellowBall : public Renderable {
public:
    YellowBall(std::shared_ptr<NavMeshItem> _item);
    ~YellowBall();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;
	D3DXVECTOR3 GetSpeed() const;
	std::shared_ptr<YellowBall> seeAnyLeader() const;
	std::shared_ptr<NavMeshItem> GetCurrentNavMeshItem() const;
	void SetLeader();
	void UnsetLeader();

protected:
    D3DXVECTOR3 position;
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };

	const float angleOfVisibility = D3DXToRadian(60.0f);
	const float visibilityDistance = 100.0f;

    const float weight = 0.1f;
    const float ballSize = 0.2f;
    const D3DXVECTOR3 scale;
    const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
    D3DXVECTOR4 color{1.0f, 1.0f, 0.0f, 1.0f};

    std::shared_ptr<YellowBallAI> ai;
	bool isLeader{ false };
	std::shared_ptr<NavMeshItem> currentNavMeshItem;

	friend class YellowBallAI;
};

using YellowBallVectorType = std::vector<std::shared_ptr<YellowBall>>;