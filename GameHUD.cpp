#include "GameHUD.h"
#include "Game.h"


GameHUD::GameHUD()
{
}


GameHUD::~GameHUD()
{
}

void GameHUD::OnRender()
{
    D3DCOLOR Red = D3DCOLOR_XRGB(255, 0, 0);
    DrawRect(center.x - 20, center.y, 40, 2, Red);
    DrawRect(center.x, center.y- 20, 2, 40, Red);
}

void GameHUD::OnUpdate(float deltaTime)
{
}

bool GameHUD::OnInit()
{
    D3DVIEWPORT9 viewport;
    Game::GetInstance()->graphicDevice->GetViewport(&viewport);
    center.x = static_cast<float>(viewport.Width / 2);
    center.y = static_cast<float>(viewport.Height / 2);
    return true;
}

D3DXVECTOR3 GameHUD::GetPosition() const
{
    return D3DXVECTOR3();
}


void GameHUD::DrawRect(int x, int y, int w, int h, D3DCOLOR color) const
{
    D3DRECT BarRect = { x, y, x + w, y + h };
    Game::GetInstance()->graphicDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 0, 0);
}