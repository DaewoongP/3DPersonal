#pragma once
#ifdef _DEBUG
#include "Base.h"

BEGIN(Engine)

class CImGuiManager : public CBase
{
	DECLARE_SINGLETON(CImGuiManager)
private:
	explicit CImGuiManager() = default;
	virtual ~CImGuiManager() = default;

public:
	HRESULT Initialize(HWND _hWnd, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);
	void Tick();
	void Render();

public:
	virtual void Free() final;
};

END

#define IMGUI		GET_SINGLE(Engine::CImGuiManager)
#endif // _DEBUG