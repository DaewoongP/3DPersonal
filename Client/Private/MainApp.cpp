#include "MainApp.h"
#include "LevelLogo.h"

USING(Engine)

HRESULT Client::MainApp::Initialize()
{
	GRAPHICDESC		graphicDesc;
	ZeroMemory(&graphicDesc, sizeof(GRAPHICDESC));

	graphicDesc.hWnd = ghWnd;
	graphicDesc.ViewportSizeX = gWinSizeX;
	graphicDesc.ViewportSizeY = gWinSizeY;
	graphicDesc.WinMode = GRAPHICDESC::WINMODE::WM_WIN;

	FAILED_RETURN(GAME->Initialize(ghInst, static_cast<_uint>(LevelType::TYPEEND), graphicDesc, mDevice, mDeviceContext), E_FAIL);
	FAILED_RETURN(GAME->OpenLevel(static_cast<_uint>(LevelType::LOGO), LevelLogo::Create()), E_FAIL);

#ifdef _DEBUG
	FAILED_RETURN(GAME->AddFont(mDevice, mDeviceContext, TEXT("Font_135"), TEXT("../../Resource/Default/Fonts/135ex.spritefont")), E_FAIL);
#endif // _DEBUG

	return S_OK;
}

void Client::MainApp::Tick(_float _timeDelta)
{
	GAME->Tick(_timeDelta);
}

HRESULT Client::MainApp::Render()
{
	return GAME->Render();
}

std::unique_ptr<Client::MainApp> Client::MainApp::Create()
{
	auto instance = std::make_unique<Client::MainApp>();
	FAILED_CHECK_RETURN_MSG(instance->Initialize(), nullptr, TEXT("Failed"));
	return instance;
}
