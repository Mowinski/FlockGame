#pragma once
#include "RedBall.h"
#include <d3dx9math.h>

class Player {
public:
	Player() = default;
    Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir);
	~Player() = default;

    void onUpdate(float deltaTime);
    void onInit();

protected:
    const float eyeHeightPosition = 0.9f;
    D3DXVECTOR3 eyePosition{ 0.0f, eyeHeightPosition, 0.0f };
    D3DXVECTOR3 lookDir{ 1.0f, 0.0f, 0.0f };
    const D3DXVECTOR3 upAxis{ 0.0f, 1.0f, 0.0f };

    const float maxYAngle{ 55.0f };
	D3DXVECTOR2 mouseCenter;
    float xRotation{ 0.0f };
    float yRotation{ 0.0f };

    const float reloadTime{ 3.0f };
    float lastShootTime{ 0.0f };

    void rotate(float deltaTime);
    void move(float deltaTime);

    D3DXVECTOR3 getLeftVector(const D3DXVECTOR3 &lookDirection) const;
    inline void centerCursor() const;
    inline D3DXVECTOR2 calculateMouseDelta() const;
    inline D3DXVECTOR2 calculateMoveSpeed() const;
    D3DXVECTOR3 calculatePosition(const D3DXVECTOR3& lookDirection, float deltaTime) const;
    D3DXVECTOR3 calculateYRotateForVector(float angle) const;
	bool isOutOfMap(const D3DXVECTOR3& position) const;
	bool isMoveAllowed(const D3DXVECTOR3& position) const;
};
