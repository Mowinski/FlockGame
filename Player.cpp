#include <sstream>

#include "Player.h"
#include "CPR_Framework.h"


Player::Player()
{
}

Player::Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir) :
    eyeHeightPosition{_eyePosition.y},
    eyePosition{ _eyePosition },
    lookDir{ _lookDir }
{
}

void Player::OnUpdate(float deltaTime)
{

    Rotate(deltaTime);
    Move(deltaTime);

    D3DXVECTOR3 target{ eyePosition + lookDir };
    LookAt(eyePosition, target);
    CenterCursor();
}

void Player::OnRender()
{
}

bool Player::OnInit()
{
    CenterCursor();
    return true;
}

Player::~Player()
{
}

void Player::Rotate(float deltaTime)
{
    D3DXVECTOR2 mouseDelta = CalculateMouseDelta();
    xRotation += -mouseDelta.x * mouseSensitivity * deltaTime;
    yRotation += mouseDelta.y * mouseSensitivity * deltaTime;
    if (yRotation > 1.4) { yRotation = 1.4; }
    if (yRotation < -1.4) { yRotation = -1.4; }
    if (xRotation > D3DX_PI) { xRotation -= 2.0f * D3DX_PI; }
    if (xRotation < -D3DX_PI) { xRotation += 2.0f * D3DX_PI; }

    D3DXMATRIX rotation;
    D3DXMatrixRotationYawPitchRoll(&rotation, xRotation, 0, yRotation);
    D3DXVec3TransformCoord(&lookDir, &D3DXVECTOR3{1.0f, 0.0f, 0.0f}, &rotation);
}

void Player::Move(float deltaTime)
{
    D3DXVECTOR2 speed = CalculateMoveSpeed() * deltaTime;
    D3DXVECTOR3 leftVector = GetLeftVector();
    eyePosition += leftVector * speed.x + lookDir * speed.y;
    eyePosition.y = eyeHeightPosition;
}

void Player::CenterCursor() const
{
    SetCursorPos(mouseCenter.x, mouseCenter.y);
}

D3DXVECTOR2 Player::CalculateMouseDelta() const
{
    return mouseCenter - GetMousePosition();
}

inline D3DXVECTOR2 Player::CalculateMoveSpeed() const
{
    float vx = 0.0f, vz = 0.0f;
    if (IsKeyPressed(Key::KEY_W) || IsKeyPressed(KEY_UP)) {
        vz = moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_S) || IsKeyPressed(KEY_DOWN)) {
        vz = -moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_A) || IsKeyPressed(KEY_LEFT)) {
        vx = moveSensitivity;
    }
    if (IsKeyPressed(Key::KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        vx = -moveSensitivity;
    }
    return D3DXVECTOR2(vx, vz);
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
