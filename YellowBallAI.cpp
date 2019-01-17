#include "YellowBallAI.h"
#include "Utils.h"

#include <algorithm>
#include <sstream>

YellowBallAI::YellowBallAI(YellowBall* _actor, std::shared_ptr<NavMeshItem> item) :
    goal(item),
    actor(_actor)
{
}

D3DXVECTOR3 YellowBallAI::OnUpdate(float deltaTime)
{
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };

	if (isDangerDetected()) {
		state = YellowBallState::SCARED;
		CreateEscapePath();
	}

    if (state == YellowBallState::IDLE) {
		force = IdleUpdate(deltaTime);
    }

    if (state == YellowBallState::MOVE_TO) {
        if (timeSinceLastChangedHeight > 10.0f) {
			desiredHeight = GetRandomHeight();
            timeSinceLastChangedHeight = 0.0f;
        }
        timeSinceLastChangedHeight += deltaTime;
		force = MoveToUpdate(deltaTime);
    }

    if (state == YellowBallState::FOLLOW_LONG_DISTANCE) {
		force = FollowLongDistanceUpdate(deltaTime);
    }

    if (state == YellowBallState::FOLLOW_SHORT_DISTANCE) {
		force = FollowShortDistanceUpdate(deltaTime);
    }

	if (state == YellowBallState::SCARED) {
		force = ScaredUpdate(deltaTime);
	}

    return force;
}

D3DXVECTOR3 YellowBallAI::FollowShortDistanceUpdate(float deltaTime)
{
	
    D3DXVECTOR3 diff = getFlockSlotPosition(); - actor->GetPosition();
    desiredHeight = targetLeader->GetPosition().y;

    float distance = diff.x*diff.x + diff.z*diff.z;
    if (distance > 1.5f) {
        state = YellowBallState::FOLLOW_LONG_DISTANCE;
        path.clear();
    }

	D3DXVECTOR3 desiredPosition = getFlockSlotPosition();
	return GetSteering(desiredPosition, Game::GetInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::ScaredUpdate(float deltaTime)
{
	D3DXVECTOR3 diff = path[0]->GetPosition() - actor->GetPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;

	if (distance < 0.1f && path.size() > 0) {
		path.erase(path.begin());
	}

	if (path.size() == 0) {
		state = YellowBallState::IDLE;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 desiredPosition = path[0]->GetPosition();
	desiredPosition.y = desiredHeight;

	float speed = Game::GetInstance()->blackboard->scaredMaxYellowBallSpeed;
	if (path.size() == 3) {
		float diffSpeed = Game::GetInstance()->blackboard->scaredMaxYellowBallSpeed - Game::GetInstance()->blackboard->maxYellowBallSpeed;
		speed = Game::GetInstance()->blackboard->scaredMaxYellowBallSpeed - 0.3f * diffSpeed;
	}
	else if (path.size() == 2) {
		float diffSpeed = Game::GetInstance()->blackboard->scaredMaxYellowBallSpeed - Game::GetInstance()->blackboard->maxYellowBallSpeed;
		speed = Game::GetInstance()->blackboard->scaredMaxYellowBallSpeed - 0.6f * diffSpeed;
	}
	else if (path.size() == 1) {
		speed = Game::GetInstance()->blackboard->maxYellowBallSpeed;
	}

	return GetSteering(desiredPosition, speed);
}

float YellowBallAI::GetRandomHeight() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return heightDist(gen);
}

D3DXVECTOR3 YellowBallAI::GetSteering(const D3DXVECTOR3 & position, float maxSpeed) const
{
	D3DXVECTOR3 diff = position - actor->GetPosition();
	D3DXVec3Normalize(&diff, &diff);
	D3DXVECTOR3 desired_velocity = diff * maxSpeed;
	return desired_velocity - actor->speed;
}

bool YellowBallAI::isDangerDetected() const
{
	for (auto ball : Game::GetInstance()->blackboard->redBalls) {
		float distance{ D3DXVec3Length(&(ball->GetPosition() - actor->GetPosition())) };
		if (distance < Game::GetInstance()->blackboard->dangerousRadius) {
			return true;
		}
	}
	return false;
}

D3DXVECTOR3 YellowBallAI::getFlockSlotPosition()
{
	D3DXVECTOR3 leftDir;
	D3DXVec3Cross(&leftDir, &(targetLeader->speed), &(targetLeader->upDir));
	if (targetLeader->aquireSlot(0, actor)) {
		return targetLeader->GetPosition() - leftDir * 0.2f - targetLeader->speed * 0.2f;
	}
	else if (targetLeader->aquireSlot(1, actor)) {
		return targetLeader->GetPosition() + leftDir * 0.2f - targetLeader->speed * 0.2f;
	}
	else if (targetLeader->aquireSlot(2, actor)) {
		return targetLeader->GetPosition() - leftDir * 0.4f - targetLeader->speed * 0.4f;
	}
	else if (targetLeader->aquireSlot(3, actor)) {
		return targetLeader->GetPosition() + leftDir * 0.4f - targetLeader->speed * 0.4f;
	}

	return targetLeader->GetPosition();
}

D3DXVECTOR3 YellowBallAI::FollowLongDistanceUpdate(float deltaTime)
{
    if (path.size() == 0) {
        path = Game::GetInstance()->blackboard->getPath(actor->GetCurrentNavMeshItem(), targetLeader->GetCurrentNavMeshItem());
    }
    if (targetLeaderNavMesh != targetLeader->GetCurrentNavMeshItem()) {
        targetLeaderNavMesh = targetLeader->GetCurrentNavMeshItem();
        path.push_back(targetLeaderNavMesh);
    }

    D3DXVECTOR3 diff = targetLeader->GetPosition() - actor->GetPosition();
    float distance = diff.x*diff.x + diff.z*diff.z;

    if (distance < 1.0f) {
        state = YellowBallState::FOLLOW_SHORT_DISTANCE;
        return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
    }

    diff = path[0]->GetPosition() - actor->GetPosition();
    distance = diff.x*diff.x + diff.z*diff.z;
    if (distance < 0.1f) {
        path.erase(path.begin());
    }
	if (path.size() == 0) {
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 desiredPosition = path[0]->GetPosition();
	desiredPosition.y = targetLeader->GetPosition().y;
	return GetSteering(desiredPosition, 1.5f * Game::GetInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::MoveToUpdate(float deltaTime)
{
    D3DXVECTOR3 diff = path[0]->GetPosition() - actor->GetPosition();
    float distance = diff.x*diff.x + diff.z*diff.z;

    if (distance < 0.1f && path.size() > 0) {
        path.erase(path.begin());
    }
    if (distance < 0.1f && path.size() == 0) {
        state = YellowBallState::IDLE;
        return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
    }

    if (!actor->isLeader && targetLeader == nullptr) {
        std::shared_ptr<YellowBall> leader = actor->seeAnyLeader();
        if (leader != nullptr) {
            state = YellowBallState::FOLLOW_LONG_DISTANCE;
            targetLeader = leader;
            path = Game::GetInstance()->blackboard->getPath(actor->GetCurrentNavMeshItem(), targetLeader->GetCurrentNavMeshItem());
            return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
        }
    }
	D3DXVECTOR3 desiredPosition = path[0]->GetPosition();
	desiredPosition.y = desiredHeight;
    return GetSteering(desiredPosition, Game::GetInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::IdleUpdate(float deltaTime)
{
    SelectNewGoal();
    state = YellowBallState::MOVE_TO;
    return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
}

void YellowBallAI::nominateToLeader()
{
    state = YellowBallState::IDLE;
}

void YellowBallAI::SelectNewGoal()
{
    static float minimumDistance = (std::min)(Game::GetInstance()->city->getMapHeight(), Game::GetInstance()->city->getMapWidth()) * 0.35f;

	std::shared_ptr<NavMeshItem> currentNavMesh = Utils::getNearestNavMeshItem(actor->position);
    std::shared_ptr<NavMeshItem> newGoal = Game::GetInstance()->blackboard->getRandomNavMeshItem(minimumDistance, currentNavMesh->position.x, currentNavMesh->position.z);
    path = Game::GetInstance()->blackboard->getPath(currentNavMesh, newGoal);
    goal = newGoal;
}

void YellowBallAI::CreateEscapePath()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	path.clear();

	unsigned int escapeStepNumber = escapeLength(gen);
	std::shared_ptr<NavMeshItem> currentNavMesh = Utils::getNearestNavMeshItem(actor->position);
	while(path.size() < escapeStepNumber) {
		currentNavMesh = currentNavMesh->GetRandomNeighbor();
		auto it = std::find(path.begin(), path.end(), currentNavMesh);
		if (it == path.end()) {
			path.push_back(currentNavMesh);
		}
	}
}
