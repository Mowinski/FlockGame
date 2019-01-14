#pragma once
#include "NavMesh.h"
#include "NavMeshItem.h"
#include "YellowBall.h"

#include <memory>

enum YellowBallState {
	IDLE,
    MOVE_TO,
    SCARED,
	FOLLOW_LONG_DISTANCE,
	FOLLOW_SHORT_DISTANCE,
};

class YellowBallAI {
public:
    YellowBallAI(YellowBall* _actor, std::shared_ptr<NavMeshItem> item);
    ~YellowBallAI();

    D3DXVECTOR3 OnUpdate(float deltaTime);
	std::shared_ptr<NavMeshItem> GetCurrentNavMeshItem() const;

protected:
    void SelectNewGoal();

	D3DXVECTOR3 MoveToUpdate();
	D3DXVECTOR3 IdleUpdate();
	D3DXVECTOR3 FollowLongDistanceUpdate();
	D3DXVECTOR3 FollowShortDistanceUpdate();

    YellowBallState state;
    std::shared_ptr<NavMesh> navMesh;
    std::shared_ptr<NavMeshItem> goal;
    NavMeshItemsVectorType path;
    YellowBall* actor;
	std::shared_ptr<YellowBall> targetLeader{ nullptr };
	std::shared_ptr<NavMeshItem> targetLeaderNavMesh{ nullptr };
};

