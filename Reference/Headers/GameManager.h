#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameManager : public CBase
{
	DECLARE_SINGLETON(CGameManager)
private:
	explicit CGameManager() = default;
	virtual ~CGameManager() = default;

public:
	typedef struct tagGameDesc
	{
		std::wstring	gameName{ TEXT("") };
		HINSTANCE		hInstance{ NULL };
		HWND			hWnd{ NULL };
		_uint			width{ 1920 };
		_uint			height{ 1080 };
		WindowType		windowType{ WindowType::WIN };
		_float4			clearColor{ 0.5f, 0.5f, 0.5f, 1.f };
#ifdef _DEBUG
		_bool			showConsole{ false };
		_bool			showFPS{ false };
#endif // _DEBUG
	}GAMEDESC;

public:
	WPARAM Run(GAMEDESC& _gameDesc);

	const GAMEDESC& GetGameDesc() { return mGameDesc; }

private:
	ATOM MyRegisterClass();
	BOOL InitInstance(_int _cmdShow);

	void Tick();
#ifdef _DEBUG
	void ShowFPS();
	void ShowConsole(_int _left, _int _top, _int _width, _int _height);
#endif // _DEBUG

	static LRESULT CALLBACK WndProc(HWND _handle, UINT _message, WPARAM _wParam, LPARAM _lParam);

private:
	GAMEDESC mGameDesc;

public:
	virtual void Free() final;
};

END

#define GAME	GET_SINGLE(CGameManager)