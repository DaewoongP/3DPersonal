#pragma once
#include "Level.h"

BEGIN(Engine)

class CLevelManager : public CBase
{
	DECLARE_SINGLETON(CLevelManager)
public:
	explicit CLevelManager() = default;
	virtual ~CLevelManager() = default;

public:
	_uint GetCurrentLevelIndex() const { return mLevelIndex; }

public:
	HRESULT OpenLevel(_uint _levelIndex, CLevel* _newLevel);
	void Tick(_float _timeDelta);

private:
	CLevel*			mCurrentLevel;
	_uint			mLevelIndex;

public:
	virtual void Free() final;
};

END

#define LEVEL		GET_SINGLE(Engine::CLevelManager)