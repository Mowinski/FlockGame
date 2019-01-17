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

    D3DXVECTOR3 onUpdate(float deltaTime);
    void nominateToLeader();

protected:
    float desiredHeight{ 2.0f };
    float timeSinceLastChangedHeight{ 0.0f };
	float pathLength{ 0.0f };
	
	YellowBallState state{ YellowBallState::IDLE };
    std::shared_ptr<NavMeshItem> goal;
    NavMeshItemsVectorType path;
    YellowBall* actor;
    std::shared_ptr<YellowBall> targetLeader{ nullptr };
    std::shared_ptr<NavMeshItem> targetLeaderNavMesh{ nullptr };
    YellowBallVectorType followers;

    std::uniform_real_distribution<float> heightDist{ 1.0f, 10.0f };
	std::uniform_int_distribution<unsigned int> escapeLength{ 3, 8 };

	void selectNewGoal();
	void createEscapePath();

	D3DXVECTOR3 moveToUpdate(float deltaTime);
	D3DXVECTOR3 idleUpdate(float deltaTime);
	D3DXVECTOR3 followLongDistanceUpdate(float deltaTime);
	D3DXVECTOR3 followShortDistanceUpdate(float deltaTime);
	D3DXVECTOR3 scaredUpdate(float deltaTime);

	float getRandomHeight() const;
	D3DXVECTOR3 getSteering(const D3DXVECTOR3& position, float maxSpeed) const;
	bool isDangerDetected() const;
    D3DXVECTOR3 getFlockSlotPosition();

	friend class YellowBall;
};

