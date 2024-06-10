#include "LevelManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CLevelManager)

CLevelManager::CLevelManager()
	: mCurrentLevel(nullptr)
	, mLevelIndex(0)
{
}

HRESULT CLevelManager::OpenLevel(_uint _levelIndex, CLevel* _newLevel)
{
	mLevelIndex = _levelIndex;

	if (nullptr != mCurrentLevel)
		Utility::SafeRelease(mCurrentLevel);

	mCurrentLevel = _newLevel;

	return S_OK;
}

void CLevelManager::Tick(_float _timeDelta)
{
	if (nullptr != mCurrentLevel)
		mCurrentLevel->Tick(_timeDelta);
}

void CLevelManager::Free()
{
	Utility::SafeRelease(mCurrentLevel);
}
