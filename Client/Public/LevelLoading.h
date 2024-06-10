#pragma once
#include "Level.h"
#include "Loader.h"

BEGIN(Client)

class CLevelLoading : public Engine::CLevel
{
public:
	explicit CLevelLoading() = default;
	virtual ~CLevelLoading() = default;

public:
	HRESULT Initialize(LevelType _levelType);
	virtual void Tick(_float _timeDelta) override;

private:
	CLoader*	mLoader;
	LevelType	mNextLevelType;

public:
	static CLevelLoading* Create(LevelType _nextLevelType);
	virtual void Free() final;
};

END