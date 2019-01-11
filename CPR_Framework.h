// Copyright (c) 2018 CD Projekt Red. All Rights Reserved.

#pragma once
#include <d3dx9.h>

//----------------------------------------------------------------------------
//  Simple functions for handling mouse input
//----------------------------------------------------------------------------

D3DXVECTOR2 GetMousePosition();
bool LeftMouseButton();
bool RightMouseButton();

//----------------------------------------------------------------------------
//  Simple function for handling keyboard input
//----------------------------------------------------------------------------

enum Key {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_SPACE,
    KEY_RETURN,
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D
};

bool IsKeyPressed(Key key);

//----------------------------------------------------------------------------
//  A function for controlling the camera
//----------------------------------------------------------------------------

void LookAt(const D3DXVECTOR3& _eye, const D3DXVECTOR3& _target);

//----------------------------------------------------------------------------
//  Mesh resource handling functions
//----------------------------------------------------------------------------

struct Mesh {
    ID3DXMesh*          m_mesh;
    int                 m_numSubsets;

private:
    ~Mesh() {};
};

Mesh* LoadFromFile(char filename[]);
void Render(Mesh* mesh, const D3DXVECTOR3& _position, const D3DXVECTOR3& _rotation, const D3DXVECTOR3& _scale, D3DXVECTOR4 _color);
void Release(Mesh* mesh);
//----------------------------------------------------------------------------
//  Entry macro
//----------------------------------------------------------------------------

typedef void(*OnInitFunc)();
typedef void(*OnShutdownFunc)();
typedef void(*OnUpdateFunc)(float);
typedef void(*OnRenderFunc)();

#define CPR_MAIN( INIT, SHUTDOWN, UPDATE, RENDER )

#include "CPR_Framework.inl"
