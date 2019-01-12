#pragma once
#include "Renderable.h"
#include <d3dx9.h>

class GameHUD : public Renderable {
public:
    GameHUD();
    ~GameHUD();

    void OnRender();
    void OnUpdate(float deltaTime);
    bool OnInit();
    D3DXVECTOR3 GetPosition() const override;


protected:
    D3DXVECTOR2 center;

    void DrawRect(int x, int y, int w, int h, D3DCOLOR color) const;
};

