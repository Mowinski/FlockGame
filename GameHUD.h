#pragma once
#include "Renderable.h"
#include <d3dx9.h>


class GameHUD : public Renderable {
public:
    GameHUD() = default;
    ~GameHUD();

	bool onInit() override;
	void onUpdate(float deltaTime) override;
    void onRender() override;

protected:
    D3DXVECTOR2 crosshairPlace;
	D3DXVECTOR2 ammoPlace;
	D3DXVECTOR2 birdCounterPlace;
	D3DXVECTOR2 timerPlace;
	ID3DXFont *font{ nullptr };
	const D3DCOLOR Red = D3DCOLOR_XRGB(255, 0, 0);

	void DrawTimer();
	void DrawBirdCounter();
	void DrawAmmoIndicator();
	void DrawCrosshair();
    void DrawRect(int x, int y, int w, int h, D3DCOLOR color) const;
};
