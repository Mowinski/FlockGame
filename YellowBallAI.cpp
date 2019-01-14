#include "YellowBallAI.h"
#include "Game.h"
#include <algorithm>
#include <random>

YellowBallAI::YellowBallAI(YellowBall* _actor, std::shared_ptr<NavMeshItem> item) :
    navMesh(Game::GetInstance()->navMesh),
    state(YellowBallState::MOVE_TO),
    goal(item),
    actor(_actor)
{
	SelectNewGoal();
	actor->currentNavMeshItem = path[0];
}

YellowBallAI::~YellowBallAI()
{
}

#include <sstream>
D3DXVECTOR3 YellowBallAI::OnUpdate(float deltaTime)
{
    D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
    if (state == YellowBallState::IDLE) {
		speed = IdleUpdate(deltaTime);
    }

	if (state == YellowBallState::MOVE_TO) {
		speed = MoveToUpdate(deltaTime);
	}

	if (state == YellowBallState::FOLLOW_LONG_DISTANCE) {
		speed = FollowLongDistanceUpdate(deltaTime);
	}

	if (state == YellowBallState::FOLLOW_SHORT_DISTANCE) {
		speed = FollowShortDistanceUpdate(deltaTime);
	}

    return speed;
}

D3DXVECTOR3 YellowBallAI::FollowShortDistanceUpdate(float deltaTime)
{
	D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 diff = targetLeader->GetPosition() - actor->GetPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;
	if (distance >= 1.5f) {
		state = YellowBallState::FOLLOW_LONG_DISTANCE;
		path.clear();
	}
	D3DXVec3Normalize(&speed, &diff);
	return speed * distance / 1.5f;
}

D3DXVECTOR3 YellowBallAI::getFlockSlotPosition(std::shared_ptr<YellowBall> follower)
{
	return D3DXVECTOR3();
}

D3DXVECTOR3 YellowBallAI::FollowLongDistanceUpdate(float deltaTime)
{
	D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
	if (path.size() == 0) {
		path = Game::GetInstance()->blackboard->getPath(actor->currentNavMeshItem, targetLeader->GetCurrentNavMeshItem());
	}
	if (targetLeaderNavMesh != targetLeader->GetCurrentNavMeshItem()) {
		targetLeaderNavMesh = targetLeader->GetCurrentNavMeshItem();
		path.push_back(targetLeaderNavMesh);
	}

	D3DXVECTOR3 diff = targetLeader->GetPosition() - actor->GetPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;

	if (distance < 1.5f) {
		state = YellowBallState::FOLLOW_SHORT_DISTANCE;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	diff = path[0]->GetPosition() - actor->GetPosition();
	distance = diff.x*diff.x + diff.z*diff.z;
	if (distance < 0.1f) {
		path.erase(path.begin());
	}

	diff.y = 0.0f;
	D3DXVec3Normalize(&speed, &diff);
	speed *= 1.5f;
	
	return speed;
}

D3DXVECTOR3 YellowBallAI::MoveToUpdate(float deltaTime)
{
	D3DXVECTOR3 speed{ 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 diff = path[0]->GetPosition() - actor->GetPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;

	if (distance < 0.1f && path.size() > 0) {
		path.erase(path.begin());
		if (path.size() > 0) {
			actor->currentNavMeshItem = *path.begin();
		}
	}
	if (distance < 0.1f && path.size() == 0) {
		state = YellowBallState::IDLE;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	if (!actor->isLeader) {
		std::shared_ptr<YellowBall> leader = actor->seeAnyLeader();
		if (leader != nullptr) {
			state = YellowBallState::FOLLOW_LONG_DISTANCE;
			targetLeader = leader;
			path = Game::GetInstance()->blackboard->getPath(actor->currentNavMeshItem, targetLeader->GetCurrentNavMeshItem());
			return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
		}
	}
	diff.y = 0.0f;
	D3DXVec3Normalize(&speed, &diff);

	return speed;
}

D3DXVECTOR3 YellowBallAI::IdleUpdate(float deltaTime)
{
	SelectNewGoal();
	actor->currentNavMeshItem = path[0];
	state = YellowBallState::MOVE_TO;

	return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
}

std::shared_ptr<NavMeshItem> YellowBallAI::GetCurrentNavMeshItem() const
{
	return actor->currentNavMeshItem;
}

void YellowBallAI::SelectNewGoal()
{
	static float minimumDistance = (std::min)(Game::GetInstance()->city->getMapHeight(), Game::GetInstance()->city->getMapWidth()) * 0.35f;

    std::shared_ptr<NavMeshItem> newGoal = Game::GetInstance()->blackboard->getRandomNavMeshItem(minimumDistance, goal->position.x, goal->position.z);
    path = Game::GetInstance()->blackboard->getPath(goal, newGoal);
    goal = newGoal;
}
