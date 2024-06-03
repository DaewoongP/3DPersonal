#pragma once
#include "Base.h"

BEGIN(Engine)

class CGraphicManager : public CBase
{
	DECLARE_SINGLETON(CGraphicManager)
private:
	explicit CGraphicManager();
	virtual ~CGraphicManager() = default;

public:
	ID3D11Device* GetDevice() const { return mDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext; }

public:
	HRESULT Initialize(HWND _hWnd, WindowType _winType, _uint _winCX, _uint _winCY);
	HRESULT ClearBackBufferView(_float4 _clearColor);
	HRESULT ClearDepthStencilView();
	HRESULT Present();
	HRESULT BindBackBuffer();

private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mBackBufferRTV;
	ID3D11DepthStencilView* mDepthStencilView;

private:
	HRESULT ReadySwapChain(HWND _hWnd, WindowType _winType, _uint _winCX, _uint _winCY);
	HRESULT ReadyBackBufferRenderTargetView();
	HRESULT ReadyDepthStencilRenderTargetView(_uint _winCX, _uint _winCY);

public:
	virtual void Free() final;
};

END

#define GRAPHIC			GET_SINGLE(Engine::CGraphicManager)