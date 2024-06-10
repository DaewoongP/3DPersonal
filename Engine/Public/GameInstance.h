#pragma once
#include "InputManager.h"

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
		_bool			showImGui{ false };
#endif // _DEBUG
	}GAMEDESC;

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

public: // Device
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

public: // Input
	_bool KeyInput(_ubyte _keyID, CInputManager::InputType _state = CInputManager::InputType::PRESS);
	_bool MouseInput(CInputManager::MouseKeyType _mouseID, CInputManager::InputType _state = CInputManager::InputType::PRESS);
	_long MouseMove(CInputManager::MouseMoveType _mouseMoveID);

public: // Level
	_uint GetCurrentLevelIndex() const;
	HRESULT OpenLevel(_uint _levelIndex, class CLevel* _newLevel);

public:
	const GAMEDESC& GetGameDesc() { return mGameDesc; }
	HRESULT Initialize(GAMEDESC& _gameDesc, _uint numLevels);
	WPARAM Run();
	
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
	class CInputManager*	mInput;
	class CLevelManager*	mLevel;

#ifdef _DEBUG
private:
	class CImGuiManager*	mImGui;
#endif // _DEBUG

public:
	virtual void Free() final;
};

END

#define GAME	GET_SINGLE(Engine::CGameInstance)