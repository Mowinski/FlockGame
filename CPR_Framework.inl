// Copyright (c) 2018 CD Projekt Red. All Rights Reserved.

typedef D3DXVECTOR2(*GetMousePositionFunc)();
extern GetMousePositionFunc GetMousePositionPtr;

typedef bool (*LeftMouseButtonFunc)();
extern LeftMouseButtonFunc LeftMouseButtonPtr;

typedef bool (*RightMouseButtonFunc)();
extern RightMouseButtonFunc RightMouseButtonPtr;

typedef bool (*IsKeyPressedFunc)(Key key);
extern IsKeyPressedFunc IsKeyPressedPtr;

typedef void (*LookAtFunc)(const D3DXVECTOR3& _eye, const D3DXVECTOR3& _target);
extern LookAtFunc LookAtPtr;

typedef Mesh* (*LoadFromFileFunc)(char filename[]);
extern LoadFromFileFunc LoadFromFilePtr;

typedef void (*RenderFunc)(Mesh* mesh, const D3DXVECTOR3& _position, const D3DXVECTOR3& _rotation, const D3DXVECTOR3& _scale, D3DXVECTOR4 _color);
extern RenderFunc RenderPtr;

typedef void(*ReleaseFunc)(Mesh* mesh);
extern ReleaseFunc ReleasePtr;

typedef int (*CPRFrameworkMainFunc)(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow,
	OnInitFunc onInitPtr,
	OnShutdownFunc onShutdownPtr,
	OnUpdateFunc onUpdatePtr,
	OnRenderFunc onRenderPtr
);

extern CPRFrameworkMainFunc CPRFrameworkMainPtr;

inline D3DXVECTOR2 GetMousePosition()
{
	return GetMousePositionPtr();
}

inline bool LeftMouseButton()
{
	return LeftMouseButtonPtr();
}

inline bool RightMouseButton()
{
	return RightMouseButtonPtr();
}

inline bool IsKeyPressed(Key key)
{
	return IsKeyPressedPtr(key);
}

inline void LookAt(const D3DXVECTOR3& _eye, const D3DXVECTOR3& _target)
{
	LookAtPtr(_eye, _target);
}

inline Mesh* LoadFromFile(char filename[])
{
	return LoadFromFilePtr(filename);
}

inline void Render(Mesh* mesh, const D3DXVECTOR3& _position, const D3DXVECTOR3& _rotation, const D3DXVECTOR3& _scale, D3DXVECTOR4 _color)
{
	RenderPtr(mesh, _position, _rotation, _scale, _color);
}

inline void Release(Mesh* mesh)
{
	ReleasePtr(mesh);
}

inline int CPRFrameworkMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow,
	OnInitFunc onInitPtr,
	OnShutdownFunc onShutdownPtr,
	OnUpdateFunc onUpdatePtr,
	OnRenderFunc onRenderPtr
)
{
	return CPRFrameworkMainPtr(hInstance,
		hPrevInstance,
		lpCmdLine,
		nCmdShow,
		onInitPtr,
		onShutdownPtr,
		onUpdatePtr,
		onRenderPtr);
}
#undef CPR_MAIN
#define CPR_MAIN( INIT, SHUTDOWN, UPDATE, RENDER )																			\
																															\
GetMousePositionFunc GetMousePositionPtr = nullptr;																			\
LeftMouseButtonFunc LeftMouseButtonPtr = nullptr;																			\
RightMouseButtonFunc RightMouseButtonPtr = nullptr;																			\
IsKeyPressedFunc IsKeyPressedPtr = nullptr;																					\
LookAtFunc LookAtPtr = nullptr;																								\
LoadFromFileFunc LoadFromFilePtr = nullptr;																					\
RenderFunc RenderPtr = nullptr;																								\
ReleaseFunc ReleasePtr = nullptr;																							\
CPRFrameworkMainFunc CPRFrameworkMainPtr = nullptr;																			\
																															\
int APIENTRY WinMain(_In_ HINSTANCE hInstance,																				\
	_In_opt_ HINSTANCE hPrevInstance,																						\
	_In_ LPSTR    lpCmdLine,																								\
	_In_ int       nCmdShow)																								\
{																															\
	HMODULE cpr_framework = LoadLibrary("CPR_Framework.dll");																\
	if (cpr_framework)																										\
	{																														\
		GetMousePositionPtr = reinterpret_cast< GetMousePositionFunc >(GetProcAddress(cpr_framework, "GetMousePosition"));	\
																															\
		LeftMouseButtonPtr = reinterpret_cast< LeftMouseButtonFunc >(GetProcAddress(cpr_framework, "LeftMouseButton"));		\
		RightMouseButtonPtr = reinterpret_cast< RightMouseButtonFunc >(GetProcAddress(cpr_framework, "RightMouseButton"));	\
																															\
		IsKeyPressedPtr = reinterpret_cast< IsKeyPressedFunc >(GetProcAddress(cpr_framework, "IsKeyPressed"));				\
																															\
		LookAtPtr = reinterpret_cast< LookAtFunc >(GetProcAddress(cpr_framework, "LookAt"));								\
																															\
		LoadFromFilePtr = reinterpret_cast< LoadFromFileFunc >(GetProcAddress(cpr_framework, "LoadFromFile"));				\
																															\
		RenderPtr = reinterpret_cast< RenderFunc > (GetProcAddress(cpr_framework, "Render"));								\
																															\
		ReleasePtr = reinterpret_cast< ReleaseFunc > (GetProcAddress(cpr_framework, "Release"));							\
																															\
		CPRFrameworkMainPtr = reinterpret_cast< CPRFrameworkMainFunc > (GetProcAddress(cpr_framework, "CPRFrameworkMain"));	\
	}																														\
																															\
	return CPRFrameworkMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow, &INIT, &SHUTDOWN, &UPDATE, &RENDER);				\
}
