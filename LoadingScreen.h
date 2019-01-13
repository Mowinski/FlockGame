#pragma once
#include "Renderable.h"
#include <string>

enum LoadingState {
    INIT,
    LOADER,
    PLAYER,
    GAME_HUD,
    PREPARE_LEVEL,
    PREPARE_NAV_MESH,
    PREPARE_YELLOW_BALL,
    PREPARE_BLACKBOARD,
    GAME_IS_READY,
};

class LoadingScreen : public Renderable {
public:
    LoadingScreen();
    ~LoadingScreen();

    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    bool OnInit() override;
    D3DXVECTOR3 GetPosition() const override;

    LoadingState state{ INIT };
protected:
    ID3DXFont *font{ nullptr };
    int width;
    int height;
    std::string task{ "" };
};

