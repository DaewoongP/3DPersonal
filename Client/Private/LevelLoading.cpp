#include "LevelLoading.h"
#include "GameInstance.h"

#include "Loader.h"
#include "LevelLogo.h"
#include "LevelPlay1.h"

HRESULT Client::CLevelLoading::Initialize(LevelType _levelType)
{
	mNextLevelType = _levelType;

	mLoader = Client::CLoader::Create(_levelType);

	return S_OK;
}

void Client::CLevelLoading::Tick(_float _timeDelta)
{
	DebugFunc::Text("Loading...", _timeDelta);

	if (false == mLoader->IsFinished())
		return;

	CLevel* level{ nullptr };

	switch (mNextLevelType)
	{
	case LevelType::LOGO:
		level = Client::CLevelLogo::Create();
		break;
	case LevelType::PLAY1:
		level = Client::CLevelPlay1::Create();
		break;
	}

	FAILED_RETURN(GAME->OpenLevel(static_cast<_uint>(mNextLevelType), level), );
}

Client::CLevelLoading* Client::CLevelLoading::Create(LevelType _nextLevelType)
{
	auto instance = _new CLevelLoading;

	NULL_CHECK_RETURN_MSG(instance, nullptr, TEXT("new instance nullptr"));
	FAILED_CHECK_RETURN_MSG(instance->Initialize(_nextLevelType), nullptr, TEXT("Create Failed"))

	return instance;
}

void Client::CLevelLoading::Free()
{
	Utility::SafeRelease(mLoader);
}
