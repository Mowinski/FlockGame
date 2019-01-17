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
    LoadingScreen() = default;
    ~LoadingScreen();

    void onRender() override;
    void onUpdate(float deltaTime) override;
    bool onInit() override;

    LoadingState state{ INIT };
protected:
    int screenWidth;
    int screenHeight;
    std::string task{ "" };

	ID3DXFont *font{ nullptr };

};

