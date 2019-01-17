#include "YellowBallAI.h"
#include "Utils.h"

#include <algorithm>
#include <sstream>

YellowBallAI::YellowBallAI(YellowBall* _actor) : 
	actor(_actor)
{
}

D3DXVECTOR3 YellowBallAI::onUpdate(float deltaTime)
{
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };

	if (isDangerDetected()) {
		state = YellowBallState::SCARED;
		createEscapePath();
	}

    if (state == YellowBallState::IDLE) {
		force = idleUpdate(deltaTime);
    }

    if (state == YellowBallState::MOVE_TO) {
        if (timeSinceLastChangedHeight > 10.0f) {
			desiredHeight = getRandomHeight();
            timeSinceLastChangedHeight = 0.0f;
        }
        timeSinceLastChangedHeight += deltaTime;
		force = moveToUpdate(deltaTime);
    }

    if (state == YellowBallState::FOLLOW_LONG_DISTANCE) {
		force = followLongDistanceUpdate(deltaTime);
    }

    if (state == YellowBallState::FOLLOW_SHORT_DISTANCE) {
		force = followShortDistanceUpdate(deltaTime);
    }

	if (state == YellowBallState::SCARED) {
		force = scaredUpdate(deltaTime);
	}

    return force;
}

void YellowBallAI::selectNewGoal()
{
	static float minimumDistance = (std::min)(Game::getInstance()->city->getMapHeight(), Game::getInstance()->city->getMapWidth()) * 0.35f;

	std::shared_ptr<NavMeshItem> currentNavMesh = Utils::getNearestNavMeshItem(actor->position);
	std::shared_ptr<NavMeshItem> newGoal = Game::getInstance()->blackboard->getRandomNavMeshItem(
		minimumDistance,
		currentNavMesh->GetPosition().x,
		currentNavMesh->GetPosition().z);
	path = Game::getInstance()->blackboard->getPath(currentNavMesh, newGoal);
}

void YellowBallAI::createEscapePath()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	path.clear();

	unsigned int escapeStepNumber = escapeLength(gen);
	std::shared_ptr<NavMeshItem> currentNavMesh = Utils::getNearestNavMeshItem(actor->position);
	while (path.size() < escapeStepNumber) {
		currentNavMesh = currentNavMesh->GetRandomNeighbor();
		auto it = std::find(path.begin(), path.end(), currentNavMesh);
		if (it == path.end()) {
			path.push_back(currentNavMesh);
		}
	}
}

D3DXVECTOR3 YellowBallAI::moveToUpdate(float deltaTime)
{
	D3DXVECTOR3 diff = path[0]->GetPosition() - actor->getPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;

	if (distance < 0.1f && path.size() > 0) {
		path.erase(path.begin());
	}
	if (distance < 0.1f && path.size() == 0) {
		state = YellowBallState::IDLE;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	if (!actor->isLeader && targetLeader == nullptr) {
		std::shared_ptr<YellowBall> leader = actor->getNearestLeaderIfAny();
		if (leader != nullptr) {
			state = YellowBallState::FOLLOW_LONG_DISTANCE;
			targetLeader = leader;
			path = Game::getInstance()->blackboard->getPath(actor->getCurrentNavMeshItem(), targetLeader->getCurrentNavMeshItem());
			return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
		}
	}
	D3DXVECTOR3 desiredPosition = path[0]->GetPosition();
	desiredPosition.y = desiredHeight;
	return getSteering(desiredPosition, Game::getInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::idleUpdate(float deltaTime)
{
	selectNewGoal();
	state = YellowBallState::MOVE_TO;
	return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
}

D3DXVECTOR3 YellowBallAI::followLongDistanceUpdate(float deltaTime)
{
	if (path.size() == 0) {
		path = Game::getInstance()->blackboard->getPath(actor->getCurrentNavMeshItem(), targetLeader->getCurrentNavMeshItem());
	}
	if (targetLeaderNavMesh != targetLeader->getCurrentNavMeshItem()) {
		targetLeaderNavMesh = targetLeader->getCurrentNavMeshItem();
		path.push_back(targetLeaderNavMesh);
	}

	D3DXVECTOR3 diff = targetLeader->getPosition() - actor->getPosition();
	float distance = diff.x*diff.x + diff.z*diff.z;

	if (distance < 1.0f) {
		state = YellowBallState::FOLLOW_SHORT_DISTANCE;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	diff = path[0]->GetPosition() - actor->getPosition();
	distance = diff.x*diff.x + diff.z*diff.z;
	if (distance < 0.1f) {
		path.erase(path.begin());
	}
	if (path.size() == 0) {
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 desiredPosition = path[0]->GetPosition();
	desiredPosition.y = targetLeader->getPosition().y;
	return getSteering(desiredPosition, 1.5f * Game::getInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::followShortDistanceUpdate(float deltaTime)
{
	
    D3DXVECTOR3 diff = getFlockSlotPosition() - actor->getPosition();
    desiredHeight = targetLeader->getPosition().y;

    float distance = diff.x*diff.x + diff.z*diff.z;
    if (distance > 1.5f) {
        state = YellowBallState::FOLLOW_LONG_DISTANCE;
        path.clear();
    }

	D3DXVECTOR3 desiredPosition = getFlockSlotPosition();
	return getSteering(desiredPosition, Game::getInstance()->blackboard->maxYellowBallSpeed);
}

D3DXVECTOR3 YellowBallAI::scaredUpdate(float deltaTime)
{
	D3DXVECTOR3 diff = path[0]->GetPosition() - actor->getPosition();
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

	float speed = Game::getInstance()->blackboard->scaredMaxYellowBallSpeed;
	if (path.size() == 3) {
		float diffSpeed = Game::getInstance()->blackboard->scaredMaxYellowBallSpeed - Game::getInstance()->blackboard->maxYellowBallSpeed;
		speed = Game::getInstance()->blackboard->scaredMaxYellowBallSpeed - 0.3f * diffSpeed;
	}
	else if (path.size() == 2) {
		float diffSpeed = Game::getInstance()->blackboard->scaredMaxYellowBallSpeed - Game::getInstance()->blackboard->maxYellowBallSpeed;
		speed = Game::getInstance()->blackboard->scaredMaxYellowBallSpeed - 0.6f * diffSpeed;
	}
	else if (path.size() == 1) {
		speed = Game::getInstance()->blackboard->maxYellowBallSpeed;
	}

	return getSteering(desiredPosition, speed);
}

float YellowBallAI::getRandomHeight() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return heightDist(gen);
}

D3DXVECTOR3 YellowBallAI::getSteering(const D3DXVECTOR3 & position, float maxSpeed) const
{
	D3DXVECTOR3 diff = position - actor->getPosition();
	D3DXVec3Normalize(&diff, &diff);
	D3DXVECTOR3 desired_velocity = diff * maxSpeed;
	return desired_velocity - actor->speed;
}

bool YellowBallAI::isDangerDetected() const
{
	for (auto ball : Game::getInstance()->blackboard->redBalls) {
		float distance{ D3DXVec3Length(&(ball->getPosition() - actor->getPosition())) };
		if (distance < Game::getInstance()->blackboard->dangerousRadius) {
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
		return targetLeader->getPosition() - leftDir * 0.2f - targetLeader->speed * 0.2f;
	}
	else if (targetLeader->aquireSlot(1, actor)) {
		return targetLeader->getPosition() + leftDir * 0.2f - targetLeader->speed * 0.2f;
	}
	else if (targetLeader->aquireSlot(2, actor)) {
		return targetLeader->getPosition() - leftDir * 0.4f - targetLeader->speed * 0.4f;
	}
	else if (targetLeader->aquireSlot(3, actor)) {
		return targetLeader->getPosition() + leftDir * 0.4f - targetLeader->speed * 0.4f;
	}

	return targetLeader->getPosition();
}
