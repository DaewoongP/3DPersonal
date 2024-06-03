// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Client.h"
#include "ClientDefines.h"
#include "GameInstance.h"

//#include "MainApp.h"

USING(Client)
USING(Engine)

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CGameInstance::GAMEDESC gameDesc;
    gameDesc.gameName = TEXT("Test");
    gameDesc.width = 1920;
    gameDesc.height = 1080;
    gameDesc.hInstance = hInstance;
    gameDesc.clearColor = _float4(0.5f, 0.5f, 0.5f, 1.f);
    gameDesc.FPS = 144.f;

#ifdef _DEBUG
    gameDesc.showConsole = true;
    gameDesc.showFPS = true;
#endif // _DEBUG

    INSTANCE->Run(gameDesc, 0);

    INSTANCE->DestroyInstance();

    return 0;
}