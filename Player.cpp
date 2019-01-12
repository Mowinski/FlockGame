#include <sstream>
#include <algorithm>

#include "Player.h"
#include "Game.h"


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
    xRotation += -mouseDelta.x * mouseSensitivity * deltaTime;
    yRotation += mouseDelta.y * mouseSensitivity * deltaTime;
    if (yRotation > maxYAngle) { yRotation = maxYAngle; }
    if (yRotation < -maxYAngle) { yRotation = -maxYAngle; }
    if (xRotation > 360.0f) { xRotation -= 360.0f; }
    if (xRotation < 0.0f) { xRotation += 360.0f; }

    D3DXMATRIX matRotationX;
    D3DXMATRIX matRotationY;
    D3DXMatrixRotationAxis(&matRotationX, &upAxis, D3DXToRadian(xRotation));
    D3DXMatrixRotationAxis(&matRotationY, &GetLeftVector(), D3DXToRadian(yRotation));
    D3DXMATRIX rotation = matRotationX * matRotationY;
    D3DXVec3TransformCoord(&lookDir, &D3DXVECTOR3{1.0f, 0.0f, 0.0f}, &rotation);
}

void Player::Move(float deltaTime)
{
    D3DXVECTOR2 speed = CalculateMoveSpeed() * deltaTime;
    D3DXVECTOR3 leftVector = GetLeftVector();
    D3DXVECTOR3 newPosition{ eyePosition + leftVector * speed.x + lookDir * speed.y };
    newPosition.y = eyeHeightPosition;

    bool isCollideWithAnyBuilding = Game::GetInstance()->city->isCollideWithAnyBuilding(newPosition, 8.0f);
    if (isCollideWithAnyBuilding) { return; }

    eyePosition = newPosition;
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

inline D3DXVECTOR2 Player::CalculateMoveSpeed() const
{
    float speedAhead = 0.0f, speedSide = 0.0f;
    if (IsKeyPressed(Key::KEY_W) || IsKeyPressed(KEY_UP)) {
        speedSide = moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_S) || IsKeyPressed(KEY_DOWN)) {
        speedSide = -moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_A) || IsKeyPressed(KEY_LEFT)) {
        speedAhead = moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        speedAhead = -moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_RETURN)) {
        DEBUG_PrintEyePosition();
    }
    return D3DXVECTOR2(speedAhead, speedSide);
}

D3DXVECTOR3 Player::GetLeftVector() const
{
    D3DXVECTOR3 leftVector;
    D3DXVec3Cross(&leftVector, &lookDir, &upAxis);
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