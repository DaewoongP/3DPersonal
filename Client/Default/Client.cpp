// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Client.h"

#include "Engine_Defines.h"
#include "Client_Defines.h"

//#include "MainApp.h"

USING(Client)
USING(Engine)

#define MAX_LOADSTRING 100

#ifdef _DEBUG
void SetConsoleWindowSize(_int _width, _int _height)
{
    _tchar consoleTitle[MAX_PATH];
    GetConsoleTitle(consoleTitle, MAX_PATH);
    HWND console = FindWindow(NULL, consoleTitle);

    RECT consoleRect;
    GetWindowRect(console, &consoleRect);
    MoveWindow(console, consoleRect.left, consoleRect.top, _width, _height, TRUE);
}

void SetConsoleWindowPosition(_int _x, _int _y)
{
    _tchar consoleTitle[MAX_PATH];
    GetConsoleTitle(consoleTitle, MAX_PATH);
    HWND console = FindWindow(NULL, consoleTitle);

    RECT consoleRect;
    GetWindowRect(console, &consoleRect);
    MoveWindow(console, _x, _y, consoleRect.right - consoleRect.left, consoleRect.bottom - consoleRect.top, TRUE);
}

#endif //_DEBUG

// 전역 변수:
HINSTANCE ghInst;                                // 현재 인스턴스입니다.
HWND	ghWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    if (::AllocConsole() == TRUE)
    {
        FILE* nfp[3];
        freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
        freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
        freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
        std::ios::sync_with_stdio();
    }

    _int consoleCX = 800, consoleCY = 600;
    // Set console size
    SetConsoleWindowSize(consoleCX, consoleCY);
    // Set console position
    SetConsoleWindowPosition(0, 0);
#endif // _DEBUG

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // Create MainApp
    /*std::unique_ptr<MainApp> mainApp = MainApp::Create();
    _float timeAcc = 0.f;

    FAILED_RETURN(GAME->AddTimer(TEXT("Timer_Default")), FALSE);
    FAILED_RETURN(GAME->AddTimer(TEXT("Timer_60")), FALSE);*/

    // Main Loop
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        //GAME->TickTimeDelta(TEXT("Timer_Default"));
        //timeAcc += GAME->GetTimeDelta(TEXT("Timer_Default"));

        ///* MainApp 객체의 처리. */
        //if (timeAcc >= 1.f / CLIENT_FRAME)
        //{
        //    GAME->TickTimeDelta(TEXT("Timer_60"));

        //    mainApp->Tick(GAME->GetTimeDelta(TEXT("Timer_60")));
        //    mainApp->Render();

        //    timeAcc = 0.f;
        //}
    }

    //GAME->Release();

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   ghInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT	windowRect = { 0, 0, gWinSizeX, gWinSizeY };

   // imgui설정할때 윈도우 사이즈값 맞춰줌
   AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ghWnd = hWnd;

   return TRUE;
}

// 이거 뿐만 아니라 위에 AdjustWindowRect도 맞춰줘야함
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;*/

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
