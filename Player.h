#pragma once
#include "RedBall.h"
#include "Renderable.h"
#include <d3dx9.h>

class Player : public Renderable {
public:
    Player();
    Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir);
	~Player();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

protected:
    const float eyeHeightPosition = 0.9f;

    D3DXVECTOR3 eyePosition{ 0.0f, eyeHeightPosition, 0.0f };
    D3DXVECTOR3 lookDir{ 1.0f, 0.0f, 0.0f };

    

    const D3DXVECTOR3 upAxis{ 0.0f, 1.0f, 0.0f };
    const float maxYAngle{ 55.0f };
    D3DXVECTOR2 mouseCenter{ 100.0f, 200.0f };
    float xRotation{ 0.0f };
    float yRotation{ 0.0f };

    const float reloadTime{ 3.0f };
    float lastShootTime{ 0.0f };

    void Rotate(float deltaTime);
    void Move(float deltaTime);

    inline D3DXVECTOR3 GetLeftVector(const D3DXVECTOR3 &lookDirection) const;
    inline void CenterCursor() const;
    inline D3DXVECTOR2 CalculateMouseDelta() const;
    inline D3DXVECTOR2 CalculateMoveSpeed();
    D3DXVECTOR3 CalculatePosition(const D3DXVECTOR3& lookDirection, float deltaTime);
    D3DXVECTOR3 CalculateYRotateForVector(float angle) const;
	bool isOutOfMap(const D3DXVECTOR3& position) const;
	bool isMoveAllowed(const D3DXVECTOR3& position) const;
    void DEBUG_PrintEyePosition() const;
    void DEBUG_PrintLookDir() const;
    void DEBUG_PrintRotator() const;
};