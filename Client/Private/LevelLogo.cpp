#include "LevelLogo.h"
#include "GameInstance.h"
#include "LevelLoading.h"

HRESULT Client::CLevelLogo::Initialize()
{
	return S_OK;
}

void Client::CLevelLogo::Tick(_float _timeDelta)
{
	DebugFunc::Text("Logo - Level", _timeDelta);
	
	if (GAME->KeyInput(DIK_RETURN, Engine::CInputManager::InputType::DOWN))
	{
		DebugFunc::Text("Enter!");
		FAILED_RETURN(GAME->OpenLevel(static_cast<_uint>(LevelType::LOADING), Client::CLevelLoading::Create(LevelType::PLAY1)), );
	}
}

Client::CLevelLogo* Client::CLevelLogo::Create()
{
	auto instance = _new CLevelLogo;

	NULL_CHECK_RETURN_MSG(instance, nullptr, TEXT("new instance nullptr"));
	FAILED_CHECK_RETURN_MSG(instance->Initialize(), nullptr, TEXT("Create Failed"))

	return instance;
}

void Client::CLevelLogo::Free()
{
}
