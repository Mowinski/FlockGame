#pragma once
#include <d3dx9.h>

#include "Renderable.h"

class Player : public Renderable {
public:
    Player();
    Player(D3DXVECTOR3 _eyePosition, D3DXVECTOR3 _lookDir);

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    bool OnInit() override;
    ~Player();

protected:
    const float eyeHeightPosition = 1.0f;

    D3DXVECTOR3 eyePosition{ 0.0f, eyeHeightPosition, 0.0f };
    D3DXVECTOR3 lookDir{ 1.0f, 0.0f, 0.0f };
    float mouseSensitivity{ 2.0f };
    float moveSensitivity{ 5.0f };

    const D3DXVECTOR3 upAxis{ 0.0f, 1.0f, 0.0f };
    D3DXVECTOR2 mouseCenter{ 100.0f, 200.0f };
    float xRotation{ 0.0f };
    float yRotation{ 0.0f };

    void Rotate(float deltaTime);
    void Move(float deltaTime);

    inline D3DXVECTOR3 GetLeftVector() const;

    inline void CenterCursor() const;
    inline D3DXVECTOR2 CalculateMouseDelta() const;
    inline D3DXVECTOR2 CalculateMoveSpeed() const;

    void DEBUG_PrintEyePosition() const;
    void DEBUG_PrintLookDir() const;
};

