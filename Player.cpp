#include "Player.h"
#include "Game.h"

#include <algorithm>
#include <sstream>


Player::Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir) :
    eyeHeightPosition{_eyePosition.y},
    eyePosition{ _eyePosition },
    lookDir{ _lookDir }
{
}

void Player::onUpdate(float deltaTime)
{
    rotate(deltaTime);
    move(deltaTime);

    D3DXVECTOR3 target{ eyePosition + lookDir };
    LookAt(eyePosition, target);

    if (Game::getInstance()->blackboard->isPlayerReloading && lastShootTime < reloadTime) {
        lastShootTime += deltaTime;
    } else {
        Game::getInstance()->blackboard->isPlayerReloading = false;
        lastShootTime = 0.0f;
    }
}

void Player::onInit()
{
	D3DVIEWPORT9 viewport;
	Game::getInstance()->graphicDevice->GetViewport(&viewport);
	mouseCenter.x = static_cast<float>(viewport.Width / 2);
	mouseCenter.y = static_cast<float>(viewport.Height / 2);
    centerCursor();
}

void Player::rotate(float deltaTime)
{
    D3DXVECTOR2 mouseDelta = calculateMouseDelta();
    xRotation += -mouseDelta.x * Game::getInstance()->blackboard->mouseSensitivity * deltaTime;
    yRotation += mouseDelta.y * Game::getInstance()->blackboard->mouseSensitivity * deltaTime;

    if (yRotation > maxYAngle) { yRotation = maxYAngle; }
    if (yRotation < -maxYAngle) { yRotation = -maxYAngle; }
    if (xRotation > 360.0f) { xRotation -= 360.0f; }
    if (xRotation < 0.0f) { xRotation += 360.0f; }

	D3DXMATRIX rotation{};
	D3DXMatrixRotationYawPitchRoll(&rotation, D3DXToRadian(xRotation), 0, D3DXToRadian(yRotation));
    D3DXVec3TransformCoord(&lookDir, &D3DXVECTOR3{1.0f, 0.0f, 0.0f}, &rotation);
}

void Player::move(float deltaTime)
{
    D3DXVECTOR3 newPosition = calculatePosition(lookDir, deltaTime);

	if (isMoveAllowed(newPosition)) {
		eyePosition = newPosition;
		return; 
    }

    const float sweepInterval = 10.0f;
    for (float angle = sweepInterval; angle <= 80; angle += sweepInterval) {
        for (int multiplier = -1; multiplier <= 1; multiplier += 2) {
            D3DXVECTOR3 newLookDir = calculateYRotateForVector(angle*multiplier);
            newPosition = calculatePosition(newLookDir, deltaTime);

            if (isMoveAllowed(newPosition)) {
                eyePosition = newPosition;
                return;
            }
        }
    }
}

D3DXVECTOR3 Player::getLeftVector(const D3DXVECTOR3 &lookDirection) const
{
	D3DXVECTOR3 leftVector{};
	D3DXVec3Cross(&leftVector, &lookDirection, &upAxis);
	return leftVector;
}

void Player::centerCursor() const
{
    SetCursorPos(static_cast<int>(mouseCenter.x), static_cast<int>(mouseCenter.y));
}

D3DXVECTOR2 Player::calculateMouseDelta() const
{
    D3DXVECTOR2 delta = mouseCenter - GetMousePosition();
    centerCursor();
    return delta;
}

inline D3DXVECTOR2 Player::calculateMoveSpeed() const
{
    float speedAhead = 0.0f, speedSide = 0.0f;
    if (IsKeyPressed(Key::KEY_W) || IsKeyPressed(KEY_UP)) {
        speedSide = Game::getInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_S) || IsKeyPressed(KEY_DOWN)) {
        speedSide = -Game::getInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_A) || IsKeyPressed(KEY_LEFT)) {
        speedAhead = Game::getInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        speedAhead = -Game::getInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (LeftMouseButton() && !Game::getInstance()->blackboard->isPlayerReloading) {
        std::shared_ptr<RedBall> ball = std::make_shared<RedBall>(eyePosition, lookDir);
        ball->onInit();
        Game::getInstance()->blackboard->redBalls.push_back(ball);
        Game::getInstance()->blackboard->isPlayerReloading = true;
    }
    return D3DXVECTOR2(speedAhead, speedSide);
}

D3DXVECTOR3 Player::calculatePosition(const D3DXVECTOR3 &lookDirection, float deltaTime) const
{
    D3DXVECTOR2 speed = calculateMoveSpeed() * deltaTime;
    D3DXVECTOR3 leftVector = getLeftVector(lookDirection);
    D3DXVECTOR3 newPosition{ eyePosition + leftVector * speed.x + lookDirection * speed.y };
    newPosition.y = eyeHeightPosition;
    return newPosition;
}

D3DXVECTOR3 Player::calculateYRotateForVector(float angle) const
{
    D3DXVECTOR3 newLookDir{ lookDir };
    angle = D3DXToRadian(angle);
    newLookDir.x = lookDir.x * cos(angle) - lookDir.z * sin(angle);
    newLookDir.z = lookDir.x * sin(angle) + lookDir.z * cos(angle);
    return newLookDir;
}

bool Player::isOutOfMap(const D3DXVECTOR3 &position) const
{
	if (std::fabs(position.x) > 0.49f * Game::getInstance()->city->getMapWidth()) {
		return true;
	}

	if (std::fabs(position.z) > 0.49f * Game::getInstance()->city->getMapHeight()) {
		return true;
	}

	return false;
}

bool Player::isMoveAllowed(const D3DXVECTOR3 &position) const
{
	if (isOutOfMap(position)) return false;
	if (Game::getInstance()->city->isCollideWithAnyBuilding(position, 5.0f)) return false;
	return true;
}
