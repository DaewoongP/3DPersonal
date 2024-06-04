#pragma once
#include "Base.h"

BEGIN(Engine)

class CInputManager : public CBase
{
	DECLARE_SINGLETON(CInputManager)
public:
	enum MOUSEKEYSTATE { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_END };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };
	enum KEYSTATE { KEY_DOWN, KEY_PRESSING, KEY_UP, KEY_END };

public:
	CInputManager();
	~CInputManager() = default;

public:
	_bool		KeyInput(_ubyte _keyID, KEYSTATE _state = KEY_PRESSING);
	_bool		MouseInput(MOUSEKEYSTATE _mouseID, KEYSTATE _state = KEY_PRESSING);
	_long		MouseMove(MOUSEMOVESTATE _mouseMoveID);

public:
	HRESULT		Initialize(HINSTANCE _hInst, HWND _hWnd);
	void		Tick();

private:
	_bool    KeyDown(const _ubyte& _key);
	_bool    KeyPressing(const _ubyte& _key);
	_bool    KeyUp(const _ubyte& _key);

	_bool    MouseDown(MOUSEKEYSTATE _mouseID);
	_bool    MousePressing(MOUSEKEYSTATE _mouseID);
	_bool    MouseUp(MOUSEKEYSTATE _mouseID);

private:
	IDirectInput8*			mInputSDK;
	IDirectInputDevice8*	mKeyBoard;
	IDirectInputDevice8*	mMouse;

private:
	_byte                   mKeyState[MAX_DIK];
	_byte                   mPreKeyState[MAX_DIK];
	DIMOUSESTATE            mMouseState;
	DIMOUSESTATE            mPreMouseState;

public:
	virtual void Free() final;
};

END

#define INPUT		GET_SINGLE(Engine::InputDevice)