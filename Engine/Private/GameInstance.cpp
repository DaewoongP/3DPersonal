#include "GameInstance.h"
#include "GraphicManager.h"
#include "TimerManager.h"
#include "LevelManager.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif // _DEBUG

USING(Engine)

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: mFPS(TEXT(""))
	, mFrame(0)
	, mFPSAcc(0.f)
	, mGraphic(GET_SINGLE(CGraphicManager))
	, mTimer(GET_SINGLE(CTimerManager))
	, mInput(GET_SINGLE(CInputManager))
	, mLevel(GET_SINGLE(CLevelManager))
#ifdef _DEBUG
	, mImGui(GET_SINGLE(CImGuiManager))
#endif // _DEBUG

{
	Utility::SafeAddRef(mGraphic);
	Utility::SafeAddRef(mTimer);
	Utility::SafeAddRef(mInput);
	Utility::SafeAddRef(mLevel);

#ifdef _DEBUG
	Utility::SafeAddRef(mImGui);
#endif // _DEBUG
}

HRESULT CGameInstance::Initialize()
{
	FAILED_CHECK(mGraphic->Initialize(mGameDesc.hWnd, mGameDesc.windowType, mGameDesc.width, mGameDesc.height));
	FAILED_CHECK(mInput->Initialize(mGameDesc.hInstance, mGameDesc.hWnd));

#ifdef _DEBUG
	FAILED_CHECK(mImGui->Initialize(mGameDesc.hWnd, GetDevice(), GetDeviceContext()));
#endif // _DEBUG

	return S_OK;
}

ID3D11Device* CGameInstance::GetDevice() const { return mGraphic->GetDevice(); }
ID3D11DeviceContext* CGameInstance::GetDeviceContext() const { return mGraphic->GetDeviceContext(); }

_bool CGameInstance::KeyInput(_ubyte _keyID, CInputManager::InputType _state) { return mInput->KeyInput(_keyID, _state); }
_bool CGameInstance::MouseInput(CInputManager::MouseKeyType _mouseID, CInputManager::InputType _state) { return mInput->MouseInput(_mouseID, _state); }
_long CGameInstance::MouseMove(CInputManager::MouseMoveType _mouseMoveID) { return mInput->MouseMove(_mouseMoveID); }

#pragma region Engine
WPARAM CGameInstance::Run(GAMEDESC& _gameDesc, _uint numLevels)
{
	mGameDesc = _gameDesc;

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (mGameDesc.showConsole)
		ShowConsole(0, 0, 800, 600);
#endif // _DEBUG

	MyRegisterClass();

	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;

	Initialize();
	
	MSG msg{ 0 };
	_float	timeAcc{ 0.f };

	mTimer->AddTimer(TEXT("Timer_Default"));
	mTimer->AddTimer(TEXT("MainTimer"));

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		mTimer->TickTimeDelta(TEXT("Timer_Default"));
		timeAcc += mTimer->GetTimeDelta(TEXT("Timer_Default"));

		if (timeAcc >= 1.f / mGameDesc.FPS)
		{
			mTimer->TickTimeDelta(TEXT("MainTimer"));

			Tick(mTimer->GetTimeDelta(TEXT("MainTimer")));
			Render();

			timeAcc = 0.f;
		}
	}

	return msg.wParam;
}

ATOM CGameInstance::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mGameDesc.hInstance;
	wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = mGameDesc.gameName.c_str();
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassExW(&wcex);
}

BOOL CGameInstance::InitInstance(_int _cmdShow)
{
	RECT rcWindow{ 0, 0, static_cast<LONG>(mGameDesc.width), static_cast<LONG>(mGameDesc.height) };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, TRUE);

	mGameDesc.hWnd = CreateWindowW(mGameDesc.gameName.c_str(), mGameDesc.gameName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, mGameDesc.hInstance, nullptr);

	if (!mGameDesc.hWnd)
		return FALSE;

	ShowWindow(mGameDesc.hWnd, _cmdShow);
	UpdateWindow(mGameDesc.hWnd);

	return TRUE;
}

#ifdef _DEBUG
void CGameInstance::ShowFPS(_float _timeDelta)
{
	mFPSAcc += _timeDelta;

	++mFrame;

	if (1.f <= mFPSAcc)
	{
		swprintf_s(mFPS, L"FPS: %d", mFrame);
		mFrame = 0;
		mFPSAcc = 0.f;
	}

	SetWindowText(mGameDesc.hWnd, mFPS);
}

void CGameInstance::ShowConsole(_int _left, _int _top, _int _width, _int _height)
{
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

	_tchar consoleTitle[MAX_PATH];
	GetConsoleTitle(consoleTitle, MAX_PATH);
	HWND console = FindWindow(NULL, consoleTitle);

	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, _left, _top, _width, _height, TRUE);
}
#endif // _DEBUG

#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG

LRESULT CGameInstance::WndProc(HWND _handle, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(_handle, _message, _wParam, _lParam))
		return true;
#endif // _DEBUG

	switch (_message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(_handle, _message, _wParam, _lParam);
	}

	return 0;
}
#pragma endregion

void CGameInstance::Tick(_float _timeDelta)
{
#ifdef _DEBUG
	if (mGameDesc.showFPS)
		ShowFPS(_timeDelta);
	if (mGameDesc.showImGui)
		mImGui->Tick();
#endif // _DEBUG

	mInput->Tick();
	mLevel->Tick(_timeDelta);
}

void CGameInstance::Render()
{
	mGraphic->ClearBackBufferView(mGameDesc.clearColor);
	mGraphic->ClearDepthStencilView();
	//FAILED_CHECK_RETURN(m_pRenderer->Draw_RenderGroup(), E_FAIL);
	//FAILED_CHECK_RETURN(mGraphic->Render_Level(), E_FAIL);

#ifdef _DEBUG
	if (mGameDesc.showImGui)
	{
		mImGui->Render();
		mGraphic->BindBackBuffer();
	}
#endif // _DEBUG

	mGraphic->Present();
}

void CGameInstance::ReleaseEngine()
{
	mGraphic->DestroyInstance();
	mTimer->DestroyInstance();
	mInput->DestroyInstance();
	mLevel->DestroyInstance();

#ifdef _DEBUG
	mImGui->DestroyInstance();
#endif // _DEBUG
}

void CGameInstance::Free()
{
	Utility::SafeRelease(mGraphic);
	Utility::SafeRelease(mTimer);
	Utility::SafeRelease(mInput);
	Utility::SafeRelease(mLevel);

#ifdef _DEBUG
	Utility::SafeRelease(mImGui);
#endif // _DEBUG

	ReleaseEngine();
}
