#include "LevelLoading.h"
#include "GameInstance.h"

#include "LevelLogo.h"
#include "LevelPlay1.h"

Client::LevelLoading::~LevelLoading()
{
}

HRESULT Client::LevelLoading::Initialize(LevelType _levelType)
{
	mNextLevelType = _levelType;

	mLoader = Client::Loader::Create(_levelType);

	return S_OK;
}

void Client::LevelLoading::Tick(_float _timeDelta)
{
	DebugFunc::Text("Loading...", _timeDelta);

	if (false == mLoader->IsFinished())
		return;

	std::unique_ptr<Level> level;

	switch (mNextLevelType)
	{
	case LevelType::LOGO:
		level = Client::LevelLogo::Create();
		break;
	case LevelType::PLAY1:
		level = Client::LevelPlay1::Create();
		break;
	}

	FAILED_RETURN(GAME->OpenLevel(static_cast<_uint>(mNextLevelType), std::move(level)), );
}

std::unique_ptr<Client::LevelLoading> Client::LevelLoading::Create(LevelType _nextLevelType)
{
	auto instance = std::make_unique<Client::LevelLoading>();
	FAILED_CHECK_RETURN_MSG(instance->Initialize(_nextLevelType), nullptr, TEXT("Failed"));
	return instance;
}
