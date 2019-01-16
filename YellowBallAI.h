#pragma once
#include "NavMesh.h"
#include "NavMeshItem.h"
#include "YellowBall.h"
#include "Game.h"

#include <memory>
#include <random>

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
    ~YellowBallAI() = default;

    D3DXVECTOR3 OnUpdate(float deltaTime);
    void nominateToLeader();

protected:
    void SelectNewGoal();

    D3DXVECTOR3 MoveToUpdate(float deltaTime);
    D3DXVECTOR3 IdleUpdate(float deltaTime);
    D3DXVECTOR3 FollowLongDistanceUpdate(float deltaTime);
    D3DXVECTOR3 FollowShortDistanceUpdate(float deltaTime);
    float GetRandomHeight() const;
	D3DXVECTOR3 GetSteering(const D3DXVECTOR3& position, float maxSpeed) const;

    float desiredHeight{ 2.0f };
    float timeSinceLastChangedHeight{ 0.0f };
	YellowBallState state{ YellowBallState::IDLE };
	std::shared_ptr<NavMesh> navMesh{ Game::GetInstance()->navMesh };
    std::shared_ptr<NavMeshItem> goal;
    NavMeshItemsVectorType path;
    YellowBall* actor;
    std::shared_ptr<YellowBall> targetLeader{ nullptr };
    std::shared_ptr<NavMeshItem> targetLeaderNavMesh{ nullptr };
    YellowBallVectorType followers;

    std::uniform_real_distribution<float> heightDist{ 1.0f, 10.0f };

    D3DXVECTOR3 getFlockSlotPosition();

	friend class YellowBall;
};

