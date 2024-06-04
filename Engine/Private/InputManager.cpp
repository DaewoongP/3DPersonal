#include "InputManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CInputManager)

CInputManager::CInputManager()
    : mInputSDK(nullptr)
    , mKeyBoard(nullptr)
    , mMouse(nullptr)
{
    ZeroMemory(mKeyState, sizeof(mKeyState));
    ZeroMemory(&mPreKeyState, sizeof(mPreKeyState));
    ZeroMemory(&mMouseState, sizeof(DIMOUSESTATE));
    ZeroMemory(&mPreMouseState, sizeof(DIMOUSESTATE));
}

_bool CInputManager::KeyInput(_ubyte _keyID, InputType _state)
{
    switch (_state)
    {
    case InputType::DOWN:
        return KeyDown(_keyID);
    case InputType::PRESS:
        return KeyPressing(_keyID);
    case InputType::UP:
        return KeyUp(_keyID);
    default:
        return false;
    }

    return false;
}

_bool CInputManager::MouseInput(MouseKeyType _mouseID, InputType _state)
{
    switch (_state)
    {
    case InputType::DOWN:
        return MouseDown(_mouseID);
    case InputType::PRESS:
        return MousePressing(_mouseID);
    case InputType::UP:
        return MouseUp(_mouseID);
    default:
        return false;
    }

    return false;
}

_long CInputManager::MouseMove(MouseMoveType _mouseMoveID)
{
    return *(reinterpret_cast<_long*>(&mMouseState)) + static_cast<_long>(_mouseMoveID);
}

HRESULT CInputManager::Initialize(HINSTANCE _hInst, HWND _hWnd)
{
    FAILED_CHECK_RETURN(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&mInputSDK), nullptr), E_FAIL);
    FAILED_CHECK_RETURN(mInputSDK->CreateDevice(GUID_SysKeyboard, &mKeyBoard, nullptr), E_FAIL);
    FAILED_CHECK_RETURN(mKeyBoard->SetDataFormat(&c_dfDIKeyboard), E_FAIL);
    FAILED_CHECK_RETURN(mKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE), E_FAIL);
    FAILED_CHECK_RETURN(mKeyBoard->Acquire(), E_FAIL);
    FAILED_CHECK_RETURN(mInputSDK->CreateDevice(GUID_SysMouse, &mMouse, nullptr), E_FAIL);
    FAILED_CHECK_RETURN(mMouse->SetDataFormat(&c_dfDIMouse), E_FAIL);
    FAILED_CHECK_RETURN(mMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE), E_FAIL);
    FAILED_CHECK_RETURN(mMouse->Acquire(), E_FAIL);

    return S_OK;
}

void CInputManager::Tick()
{
    memcpy(mPreKeyState, mKeyState, sizeof(_byte) * MAX_DIK);
    memcpy(&mPreMouseState, &mMouseState, sizeof(DIMOUSESTATE));

    mKeyBoard->GetDeviceState(MAX_DIK, mKeyState);
    mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
}

_bool CInputManager::KeyDown(const _ubyte& _key)
{
    if (!(mPreKeyState[_key] & 0x80) && (mKeyState[_key] & 0x80))
        return true;

    return false;
}

_bool CInputManager::KeyPressing(const _ubyte& _key)
{
    if ((mPreKeyState[_key] & 0x80) && (mKeyState[_key] & 0x80))
        return true;

    return false;
}

_bool CInputManager::KeyUp(const _ubyte& _key)
{
    if ((mPreKeyState[_key] & 0x80) && !(mKeyState[_key] & 0x80))
        return true;

    return false;
}

_bool CInputManager::MouseDown(MouseKeyType _mouseID)
{
    _int id{ static_cast<_int>(_mouseID) };
    if (!mPreMouseState.rgbButtons[id] && mMouseState.rgbButtons[id])
        return true;

    return false;
}

_bool CInputManager::MousePressing(MouseKeyType _mouseID)
{
    _int id{ static_cast<_int>(_mouseID) };
    if (mPreMouseState.rgbButtons[id] && mMouseState.rgbButtons[id])
        return true;

    return false;
}

_bool CInputManager::MouseUp(MouseKeyType _mouseID)
{
    _int id{ static_cast<_int>(_mouseID) };
    if (mPreMouseState.rgbButtons[id] && !mMouseState.rgbButtons[id])
        return true;

    return false;
}

void CInputManager::Free()
{
    Utility::SafeRelease(mInputSDK);
    Utility::SafeRelease(mKeyBoard);
    Utility::SafeRelease(mMouse);
}
