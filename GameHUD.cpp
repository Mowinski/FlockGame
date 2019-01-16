#include "GameHUD.h"
#include "Game.h"

#include <cmath>
#include <sstream>
#include <iomanip> 

GameHUD::GameHUD()
{
}


GameHUD::~GameHUD()
{
	if (font != nullptr) {
		font->Release();
		font = nullptr;
	}
}

void GameHUD::OnRender()
{
	DrawCrosshair();
	DrawAmmoIndicator();
	DrawBirdCounter();
	DrawTimer();
}

void GameHUD::DrawTimer()
{
	std::ostringstream oss{};
	oss << std::setprecision(3) << std::fixed << "Time: " << Game::GetInstance()->timer->getRoundTime() << " sec";
	RECT font_rect{ static_cast<int>(timerPlace.x), static_cast<int>(timerPlace.y), static_cast<int>(timerPlace.x) + 180, static_cast<int>(timerPlace.y) + 50 };
	font->DrawText(NULL, oss.str().c_str(), -1, &font_rect, DT_VCENTER | DT_NOCLIP, Red);
}

void GameHUD::DrawBirdCounter()
{
	std::ostringstream oss{};
	oss << "# of birds: " << Game::GetInstance()->blackboard->yellowBalls.size();
	RECT font_rect{ static_cast<int>(birdCounterPlace.x), static_cast<int>(birdCounterPlace.y), static_cast<int>(birdCounterPlace.x) + 80, static_cast<int>(birdCounterPlace.y) + 50 };
	font->DrawText(NULL, oss.str().c_str(), -1, &font_rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, Red);
}

void GameHUD::DrawAmmoIndicator()
{
	RECT font_rect{ static_cast<int>(ammoPlace.x) - 80, static_cast<int>(ammoPlace.y), static_cast<int>(ammoPlace.x), static_cast<int>(ammoPlace.y) + 50 };
	font->DrawText(NULL, "Ammo", -1, &font_rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, Red);
	if (!Game::GetInstance()->blackboard->isPlayerReloading) {
		DrawRect(static_cast<int>(ammoPlace.x), static_cast<int>(ammoPlace.y), 20, 50, Red);
	}
}

void GameHUD::DrawCrosshair()
{
	DrawRect(static_cast<int>(crosshairPlace.x) - 20, static_cast<int>(crosshairPlace.y), 40, 2, Red);
	DrawRect(static_cast<int>(crosshairPlace.x), static_cast<int>(crosshairPlace.y) - 20, 2, 40, Red);
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

	ammoPlace.x = viewport.Width - 80.0f;
	ammoPlace.y = viewport.Height - 60.0f;

	birdCounterPlace.x = 40.0f;
	birdCounterPlace.y = viewport.Height - 60.0f;

	timerPlace.x = 40.0f;
	timerPlace.y = 10.0f;

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

