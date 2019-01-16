#include "Player.h"
#include "Game.h"

#include <algorithm>
#include <sstream>


Player::Player()
{
}

Player::Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir) :
    eyeHeightPosition{_eyePosition.y},
    eyePosition{ _eyePosition },
    lookDir{ _lookDir }
{
}

Player::~Player()
{
}

void Player::OnUpdate(float deltaTime)
{
    Rotate(deltaTime);
    Move(deltaTime);

    D3DXVECTOR3 target{ eyePosition + lookDir };
    LookAt(eyePosition, target);

    if (Game::GetInstance()->blackboard->isPlayerReloading && lastShootTime < reloadTime) {
        lastShootTime += deltaTime;
    } else {
        Game::GetInstance()->blackboard->isPlayerReloading = false;
        lastShootTime = 0.0f;
    }
}

void Player::OnRender()
{
}

bool Player::OnInit()
{
    CenterCursor();
    return true;
}

D3DXVECTOR3 Player::GetPosition() const
{
    return eyePosition;
}

void Player::Rotate(float deltaTime)
{
    D3DXVECTOR2 mouseDelta = CalculateMouseDelta();
    xRotation += -mouseDelta.x * Game::GetInstance()->blackboard->mouseSensitivity * deltaTime;
    yRotation += mouseDelta.y * Game::GetInstance()->blackboard->mouseSensitivity * deltaTime;

    if (yRotation > maxYAngle) { yRotation = maxYAngle; }
    if (yRotation < -maxYAngle) { yRotation = -maxYAngle; }
    if (xRotation > 360.0f) { xRotation -= 360.0f; }
    if (xRotation < 0.0f) { xRotation += 360.0f; }

	D3DXMATRIX rotation{};
	D3DXMatrixRotationYawPitchRoll(&rotation, D3DXToRadian(xRotation), 0, D3DXToRadian(yRotation));
    D3DXVec3TransformCoord(&lookDir, &D3DXVECTOR3{1.0f, 0.0f, 0.0f}, &rotation);
}

void Player::Move(float deltaTime)
{
    D3DXVECTOR3 newPosition = CalculatePosition(lookDir, deltaTime);

	if (isMoveAllowed(newPosition)) {
		eyePosition = newPosition;
		return; 
    }

    const float sweepInterval = 10.0f;
    for (float angle = sweepInterval; angle <= 80; angle += sweepInterval) {
        for (int multiplier = -1; multiplier <= 1; multiplier += 2) {
            D3DXVECTOR3 newLookDir = CalculateYRotateForVector(angle*multiplier);
            newPosition = CalculatePosition(newLookDir, deltaTime);

            if (isMoveAllowed(newPosition)) {
                eyePosition = newPosition;
                return;
            }
        }
    }
}

void Player::CenterCursor() const
{
    SetCursorPos(static_cast<int>(mouseCenter.x), static_cast<int>(mouseCenter.y));
}

D3DXVECTOR2 Player::CalculateMouseDelta() const
{
    D3DXVECTOR2 delta = mouseCenter - GetMousePosition();
    CenterCursor();
    return delta;
}

inline D3DXVECTOR2 Player::CalculateMoveSpeed()
{
    float speedAhead = 0.0f, speedSide = 0.0f;
    if (IsKeyPressed(Key::KEY_W) || IsKeyPressed(KEY_UP)) {
        speedSide = Game::GetInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_S) || IsKeyPressed(KEY_DOWN)) {
        speedSide = -Game::GetInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_A) || IsKeyPressed(KEY_LEFT)) {
        speedAhead = Game::GetInstance()->blackboard->maxMovePlayerSpeed;
    }
    if (IsKeyPressed(Key::KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        speedAhead = -Game::GetInstance()->blackboard->maxMovePlayerSpeed;
    }
	if (IsKeyPressed(Key::KEY_RETURN)) {
		DEBUG_PrintEyePosition();
	}
    if (LeftMouseButton() && !Game::GetInstance()->blackboard->isPlayerReloading) {
        std::shared_ptr<RedBall> ball = std::make_shared<RedBall>(eyePosition, lookDir);
        ball->OnInit();
        Game::GetInstance()->blackboard->redBalls.push_back(ball);
        Game::GetInstance()->blackboard->isPlayerReloading = true;
    }
    return D3DXVECTOR2(speedAhead, speedSide);
}

D3DXVECTOR3 Player::CalculatePosition(const D3DXVECTOR3 &lookDirection, float deltaTime)
{
    D3DXVECTOR2 speed = CalculateMoveSpeed() * deltaTime;
    D3DXVECTOR3 leftVector = GetLeftVector(lookDirection);
    D3DXVECTOR3 newPosition{ eyePosition + leftVector * speed.x + lookDirection * speed.y };
    newPosition.y = eyeHeightPosition;
    return newPosition;
}

D3DXVECTOR3 Player::CalculateYRotateForVector(float angle) const
{
    D3DXVECTOR3 newLookDir{ lookDir };
    angle = D3DXToRadian(angle);
    newLookDir.x = lookDir.x * cos(angle) - lookDir.z * sin(angle);
    newLookDir.z = lookDir.x * sin(angle) + lookDir.z * cos(angle);
    return newLookDir;
}

bool Player::isOutOfMap(const D3DXVECTOR3 &position) const
{
	if (std::fabs(position.x) > 0.49f * Game::GetInstance()->city->getMapWidth()) {
		return true;
	}

	if (std::fabs(position.z) > 0.49f * Game::GetInstance()->city->getMapHeight()) {
		return true;
	}

	return false;
}

bool Player::isMoveAllowed(const D3DXVECTOR3 &position) const
{
	if (isOutOfMap(position)) return false;
	if (Game::GetInstance()->city->isCollideWithAnyBuilding(position, 5.0f)) return false;
	return true;
}

D3DXVECTOR3 Player::GetLeftVector(const D3DXVECTOR3 &lookDirection) const
{
    D3DXVECTOR3 leftVector;
    D3DXVec3Cross(&leftVector, &lookDirection, &upAxis);
    return leftVector;
}

void Player::DEBUG_PrintEyePosition() const
{
    std::ostringstream oss{};
    oss << "EyePosition: {x: " << eyePosition.x << " y: " << eyePosition.y << " z: " << eyePosition.z << "}" << std::endl;
    OutputDebugString(oss.str().c_str());
}

void Player::DEBUG_PrintLookDir() const
{
    std::ostringstream oss{};
    oss << "LookDir: {x: " << lookDir.x << " y: " << lookDir.y << " z: " << lookDir.z << "}" << std::endl;
    OutputDebugString(oss.str().c_str());
}

void Player::DEBUG_PrintRotator() const
{
    std::ostringstream oss{};
    oss << "MouseRotation: {x: " << xRotation << " y: " << yRotation << "}" << std::endl;
    OutputDebugString(oss.str().c_str());
}