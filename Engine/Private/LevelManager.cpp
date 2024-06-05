#include "LevelManager.h"

USING(Engine)

HRESULT CLevelManager::OpenLevel(_uint _levelIndex, CLevel* _newLevel)
{
	mLevelIndex = _levelIndex;

	if (nullptr != mCurrentLevel)
		Utility::SafeRelease(mCurrentLevel);
	mCurrentLevel = _newLevel;
	Utility::SafeAddRef(mCurrentLevel);

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
