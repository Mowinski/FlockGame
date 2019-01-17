#include "LoadingScreen.h"
#include "Game.h"

LoadingScreen::~LoadingScreen()
{
    if (font != nullptr) {
        font->Release();
        font = nullptr;
    }
}

void LoadingScreen::onRender()
{
    std::string str = "Loading...\n" + task;

    RECT font_rect;
    SetRect(&font_rect, 0, 0, screenWidth, screenHeight);

    Game::getInstance()->graphicDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 0, 0);
    font->DrawText(NULL, str.c_str(), -1, &font_rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void LoadingScreen::onUpdate(float deltaTime)
{
    switch (state) {
    case INIT:
        task = "Initialization";
        break;
    case LOADER:
        task = "Loading meshes";
        break;
    case PLAYER:
        task = "Create player";
        break;
    case GAME_HUD:
        task = "Prepare hud";
        break;
    case PREPARE_LEVEL:
        task = "Prepare level";
        break;
    case PREPARE_NAV_MESH:
        task = "Prepare nav mesh";
        break;
    case PREPARE_YELLOW_BALL:
        task = "Create initials yellow balls";
        break;
    case PREPARE_BLACKBOARD:
        task = "Prepare blackboard for AI";
        break;
    case GAME_IS_READY:
        task = "Game is ready. Prepare yourself";
        break;
    default:
        task = "Unknown!";
		break;
    }
}

bool LoadingScreen::onInit()
{
    D3DVIEWPORT9 viewport;
    Game::getInstance()->graphicDevice->GetViewport(&viewport);
    screenWidth = viewport.Width;
    screenHeight = viewport.Height;

    HRESULT hr = D3DXCreateFont(Game::getInstance()->graphicDevice,
                                46,
                                0,
                                FW_BOLD,
                                1,
                                false,
                                DEFAULT_CHARSET,
                                OUT_DEFAULT_PRECIS,
                                ANTIALIASED_QUALITY,
                                DEFAULT_PITCH | FF_DONTCARE,
                                "Impact",
                                &font);
    return SUCCEEDED(hr);
}
