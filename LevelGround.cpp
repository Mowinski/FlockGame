#include "LevelGround.h"

#include "CPR_Framework.h"
#include "Game.h"



LevelGround::LevelGround(float _mapSizeX, float _mapSizeZ) : mapSizeX(_mapSizeX), mapSizeZ(_mapSizeZ)
{
}


LevelGround::~LevelGround()
{
}

void LevelGround::OnUpdate(float deltaTime)
{
}

void LevelGround::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitbox"), levelGroundPosition, rotation, levelGroundScale, greenColor);

	// Fence Render
	for (unsigned int i = 0; i < fencePosition.size(); ++i) {
		Render(Game::GetInstance()->loader->GetMesh("unitbox"), fencePosition[i], rotation, fenceScale[i], blueColor);
	}
}

bool LevelGround::OnInit()
{
	levelGroundScale = D3DXVECTOR3{ mapSizeX, 0.10f, mapSizeZ };
	fencePosition[0] = D3DXVECTOR3{ -0.5f * mapSizeX, 0.0f, 0.0f };
	fencePosition[1] = D3DXVECTOR3{ 0.5f * mapSizeX, 0.0f, 0.0f };
	fencePosition[2] = D3DXVECTOR3{ 0.0f, 0.0f, -0.5f * mapSizeZ };
	fencePosition[3] = D3DXVECTOR3{ 0.0f, 0.0f, 0.5f * mapSizeZ };

	fenceScale[0] = D3DXVECTOR3{ 0.2f, 1.0f, mapSizeZ };
	fenceScale[1] = D3DXVECTOR3{ 0.2f, 1.0f, mapSizeZ };
	fenceScale[2] = D3DXVECTOR3{ mapSizeX, 1.0f, 0.2f };
	fenceScale[3] = D3DXVECTOR3{ mapSizeX, 1.0f, 0.2f };

    return true;
}

D3DXVECTOR3 LevelGround::GetPosition() const
{
    return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
