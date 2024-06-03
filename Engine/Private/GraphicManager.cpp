#include "GraphicManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CGraphicManager)

CGraphicManager::CGraphicManager()
	: mDevice(nullptr)
	, mDeviceContext(nullptr)
	, mSwapChain(nullptr)
	, mBackBufferRTV(nullptr)
	, mDepthStencilView(nullptr)
{
}

HRESULT CGraphicManager::Initialize(HWND _hWnd, WindowType _winType, _uint _winCX, _uint _winCY)
{
	_uint flag{ 0 };

#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG

	D3D_FEATURE_LEVEL	featureLV;

	FAILED_CHECK_RETURN_MSG(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, nullptr, 0, D3D11_SDK_VERSION,
		&mDevice, &featureLV, &mDeviceContext), E_FAIL, TEXT("Failed Create Device"));

	FAILED_CHECK(ReadySwapChain(_hWnd, _winType, _winCX, _winCY));
	FAILED_CHECK(ReadyBackBufferRenderTargetView());
	FAILED_CHECK(ReadyDepthStencilRenderTargetView(_winCX, _winCY));
	
	ID3D11RenderTargetView* RTVs[1]{ mBackBufferRTV };
	mDeviceContext->OMSetRenderTargets(1, RTVs, mDepthStencilView);

	D3D11_VIEWPORT	viewPortDesc{ 0,0, static_cast<_float>(_winCX), static_cast<_float>(_winCY), 0.f, 1.f };
	mDeviceContext->RSSetViewports(1, &viewPortDesc);

	return S_OK;
}

HRESULT CGraphicManager::ClearBackBufferView(_float4 _clearColor)
{
	NULL_CHECK_RETURN_MSG(mDeviceContext, E_FAIL, TEXT("Device Context NULL"));

	mDeviceContext->ClearRenderTargetView(mBackBufferRTV, reinterpret_cast<_float*>(&_clearColor));

	return S_OK;
}

HRESULT CGraphicManager::ClearDepthStencilView()
{
	NULL_CHECK_RETURN_MSG(mDeviceContext, E_FAIL, TEXT("Device Context NULL"));

	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicManager::Present()
{
	NULL_CHECK_RETURN_MSG(mSwapChain, E_FAIL, TEXT("SwapChain NULL"));

	FAILED_CHECK_RETURN_MSG(mSwapChain->Present(0, 0), E_FAIL, TEXT("Failed Present"));

	return S_OK;
}

HRESULT CGraphicManager::BindBackBuffer()
{
	NULL_CHECK_RETURN_MSG(mDeviceContext, E_FAIL, TEXT("Device Context NULL"));
	NULL_CHECK_RETURN_MSG(mBackBufferRTV, E_FAIL, TEXT("BackBufferRTV NULL"));
	NULL_CHECK_RETURN_MSG(mDepthStencilView, E_FAIL, TEXT("DepthStencilView NULL"));

	mDeviceContext->OMSetRenderTargets(1, &mBackBufferRTV, mDepthStencilView);

	return S_OK;
}

HRESULT CGraphicManager::ReadySwapChain(HWND _hWnd, WindowType _winType, _uint _winCX, _uint _winCY)
{
	IDXGIDevice* device{ nullptr };
	mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));

	IDXGIAdapter* adapter{ nullptr };
	device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));

	IDXGIFactory* factory{ nullptr };
	adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	DXGI_SWAP_CHAIN_DESC	swapChain;
	ZeroMemory(&swapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChain.BufferDesc.Width = _winCX;
	swapChain.BufferDesc.Height = _winCY;
	swapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChain.BufferCount = 1;

	swapChain.BufferDesc.RefreshRate.Numerator = 60;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;
	swapChain.SampleDesc.Quality = 0;
	swapChain.SampleDesc.Count = 1;

	swapChain.OutputWindow = _hWnd;
	swapChain.Windowed = static_cast<_bool>(_winType);
	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	FAILED_CHECK_RETURN_MSG(factory->CreateSwapChain(mDevice, &swapChain, &mSwapChain), E_FAIL, TEXT("Failed CreateSwapChain"));

	Utility::SafeRelease(factory);
	Utility::SafeRelease(adapter);
	Utility::SafeRelease(device);

	return S_OK;
}

HRESULT CGraphicManager::ReadyBackBufferRenderTargetView()
{
	NULL_CHECK_RETURN_MSG(mDevice, E_FAIL, TEXT("Device NULL"));

	ID3D11Texture2D* backBufferTexture{ nullptr };

	FAILED_CHECK_RETURN_MSG(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture)), E_FAIL,
		TEXT("Failed GetBuffer"));

	FAILED_CHECK_RETURN_MSG(mDevice->CreateRenderTargetView(backBufferTexture, nullptr, &mBackBufferRTV), E_FAIL,
		TEXT("Failed Create RenderTarget"));

	Utility::SafeRelease(backBufferTexture);

	return S_OK;
}

HRESULT CGraphicManager::ReadyDepthStencilRenderTargetView(_uint _winCX, _uint _winCY)
{
	NULL_CHECK_RETURN_MSG(mDevice, E_FAIL, TEXT("Device NULL"));

	ID3D11Texture2D* depthStencilTexture{ nullptr };
	D3D11_TEXTURE2D_DESC	textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = _winCX;
	textureDesc.Height = _winCY;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	FAILED_CHECK_RETURN_MSG(mDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencilTexture), E_FAIL, TEXT("Failed Create Texture2D"));
	FAILED_CHECK_RETURN_MSG(mDevice->CreateDepthStencilView(depthStencilTexture, nullptr, &mDepthStencilView), E_FAIL,
		TEXT("Failed CreateDepthStencilView"));

	Utility::SafeRelease(depthStencilTexture);

	return S_OK;
}

void CGraphicManager::Free()
{
	Utility::SafeRelease(mSwapChain);
	Utility::SafeRelease(mBackBufferRTV);
	Utility::SafeRelease(mDeviceContext);
	Utility::SafeRelease(mDepthStencilView);

#if defined(DEBUG) || defined(_DEBUG)
	/*ID3D11Debug* d3dDebug;
	HRESULT hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	}
	if (d3dDebug != nullptr) d3dDebug->Release();*/
#endif

	Utility::SafeRelease(mDevice);
}
