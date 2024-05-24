#include "LevelLogo.h"
#include "GameInstance.h"
#include "LevelLoading.h"

HRESULT Client::LevelLogo::Initialize()
{
	return S_OK;
}

void Client::LevelLogo::Tick(_float _timeDelta)
{
	DebugFunc::Text("Logo - Level", _timeDelta);

	if (GAME->GetDIKeyState(DIK_RETURN, Engine::InputDevice::KEY_DOWN))
	{
		DebugFunc::Text("Enter!");
		FAILED_RETURN(GAME->OpenLevel(static_cast<_uint>(LevelType::LOADING), Client::LevelLoading::Create(LevelType::PLAY1)), );
	}
}

std::unique_ptr<Client::LevelLogo> Client::LevelLogo::Create()
{
	auto instance = std::make_unique<Client::LevelLogo>();
	FAILED_CHECK_RETURN_MSG(instance->Initialize(), nullptr, TEXT("Failed"));
	return instance;
}
