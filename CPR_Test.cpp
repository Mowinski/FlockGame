#include "CPR_Framework.h"
#include "Game.h"

Game*   g_game = nullptr;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	This file contains the main loop of the program and works as follows:

		OnInit();

		while( !shouldShutDown )
		{
			OnUpdate( deltaTime );
			OnRender();
		}

		OnShutdown();

	For vector & matrix math we're using the D3DX Library. 
	Here are some useful classes & functions (that may or may not be handy):

		D3DXVECTOR3		- x, y, z (floats)
		D3DXMATRIX		- 16 float values
		D3DXQUATERNION	- x, y, z, w (floats)

		D3DXVECTOR3*	D3DXVec3Normalize( D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV );
		FLOAT			D3DXVec3Length( const D3DXVECTOR3 *pV );
		FLOAT			D3DXVec3Dot( const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2 );
		D3DXVECTOR3*	D3DXVec3Cross( D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2 );

	You can find these and more in the DX SDK documentation.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//----------------------------------------------------------------------------
void OnInit()
{
	if (g_game != nullptr) return;
	g_game = new Game();
	if (!g_game->Init("resources/city.txt")) {
		MessageBox(NULL, "Cannot initialize game", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

//----------------------------------------------------------------------------
void OnShutdown()
{
	delete g_game;
}

//----------------------------------------------------------------------------
void OnUpdate(float deltaTime)
{
	g_game->Update(deltaTime);
}

//----------------------------------------------------------------------------
void OnRender()
{
	g_game->Render();
}

CPR_MAIN(OnInit, OnShutdown, OnUpdate, OnRender)
