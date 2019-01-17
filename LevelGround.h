#pragma once
#include "Renderable.h"
#include <array>

class LevelGround : public Renderable {
public:
    LevelGround(float _mapSizeX, float _mapSizeZ);
    ~LevelGround() = default;

    void onUpdate(float deltaTime) override;
    void onRender() override;
    bool onInit() override;

private:
    const float mapSizeX;
    const float mapSizeZ;

	const D3DXVECTOR3 levelGroundPosition{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 rotation{ 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR4 greenColor{ 0.0f, 1.0f, 0.0f, 1.0f };
	const D3DXVECTOR4 blueColor{ 0.0f, 0.0f, 1.0f, 1.0f };

	D3DXVECTOR3 levelGroundScale;
	std::array<D3DXVECTOR3, 4> fencePosition;
	std::array<D3DXVECTOR3, 4> fenceScale;
};

