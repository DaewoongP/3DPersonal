#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
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
		_float			FPS{ 60.f };
#ifdef _DEBUG
		_bool			showConsole{ false };
		_bool			showFPS{ false };
#endif // _DEBUG
	}GAMEDESC;

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

private:
	HRESULT Initialize();

public:
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

public:
	const GAMEDESC& GetGameDesc() { return mGameDesc; }
	WPARAM Run(GAMEDESC& _gameDesc, _uint numLevels);
	
private:
	ATOM MyRegisterClass();
	BOOL InitInstance(_int _cmdShow);
	
#ifdef _DEBUG
	void ShowFPS(_float _timeDelta);
	void ShowConsole(_int _left, _int _top, _int _width, _int _height);
#endif // _DEBUG

	static LRESULT CALLBACK WndProc(HWND _handle, UINT _message, WPARAM _wParam, LPARAM _lParam);

private:
	void Tick(_float _timeDelta);
	void Render();
	void ReleaseEngine();

private:
	GAMEDESC				mGameDesc;
	_tchar					mFPS[MAX_PATH];
	_int					mFrame;
	_float					mFPSAcc;

private:
	class CGraphicManager*	mGraphic;
	class CTimerManager*	mTimer;

#ifdef _DEBUG
private:
	class CImGuiManager*	mImGui;
#endif // _DEBUG

public:
	virtual void Free() final;
};

END

#define INSTANCE	GET_SINGLE(Engine::CGameInstance)