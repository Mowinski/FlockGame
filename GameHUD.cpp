#include "GameHUD.h"
#include "Game.h"

#include <cmath>


GameHUD::GameHUD()
{
}


GameHUD::~GameHUD()
{
}

void GameHUD::OnRender()
{
    D3DCOLOR Red = D3DCOLOR_XRGB(255, 0, 0);
    DrawRect(crosshairPlace.x - 20, crosshairPlace.y, 40, 2, Red);
    DrawRect(crosshairPlace.x, crosshairPlace.y- 20, 2, 40, Red);

	RECT font_rect{ ammoPlace.x - 80, ammoPlace.y, ammoPlace.x, ammoPlace.y + 50 };
	font->DrawText(NULL, "Ammo", -1, &font_rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, Red);
	if (!Game::GetInstance()->blackboard->isPlayerReloading) {
		DrawRect(ammoPlace.x, ammoPlace.y, 20, 50, Red);
	}
}

void GameHUD::OnUpdate(float deltaTime)
{
}

bool GameHUD::OnInit()
{
    D3DVIEWPORT9 viewport;
    Game::GetInstance()->graphicDevice->GetViewport(&viewport);
    crosshairPlace.x = static_cast<float>(viewport.Width / 2);
    crosshairPlace.y = static_cast<float>(viewport.Height / 2);

	ammoPlace.x = viewport.Width - 80;
	ammoPlace.y = viewport.Height - 60;

	HRESULT hr = D3DXCreateFont(Game::GetInstance()->graphicDevice,
		34,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Impact",
		&font);

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

