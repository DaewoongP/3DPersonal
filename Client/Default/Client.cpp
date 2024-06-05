#include "framework.h"
#include "Client.h"
#include "ClientDefines.h"
#include "GameInstance.h"

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
    gameDesc.showImGui = true;
#endif // _DEBUG

    INSTANCE->Run(gameDesc, 0);

    INSTANCE->DestroyInstance();

    return 0;
}