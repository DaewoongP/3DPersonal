#pragma once
#include "Base.h"

BEGIN(Engine)

class CInputManager : public CBase
{
	DECLARE_SINGLETON(CInputManager)
public:
	enum class MouseKeyType { LBUTTON, RBUTTON, WHEEL, TYPEEND };
	enum class MouseMoveType { X, Y, WHEEL, TYPEEND };
	enum class InputType { DOWN, PRESS, UP, TYPEEND };

public:
	CInputManager();
	~CInputManager() = default;

public:
	_bool		KeyInput(_ubyte _keyID, InputType _state = InputType::PRESS);
	_bool		MouseInput(MouseKeyType _mouseID, InputType _state = InputType::PRESS);
	_long		MouseMove(MouseMoveType _mouseMoveID);

public:
	HRESULT		Initialize(HINSTANCE _hInst, HWND _hWnd);
	void		Tick();

private:
	_bool    KeyDown(const _ubyte& _key);
	_bool    KeyPressing(const _ubyte& _key);
	_bool    KeyUp(const _ubyte& _key);

	_bool    MouseDown(MouseKeyType _mouseID);
	_bool    MousePressing(MouseKeyType _mouseID);
	_bool    MouseUp(MouseKeyType _mouseID);

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