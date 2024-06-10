#pragma once
#include "Base.h"
#include "ClientDefines.h"

BEGIN(Client)

class CLoader : public Engine::CBase
{
public:
	explicit CLoader() = default;
	virtual ~CLoader() = default;
public:
	_bool IsFinished() const { return mFinished; }

public:
	HRESULT Initialize(LevelType _levelType);
	HRESULT Loading();

private:
	HRESULT LoadingForLogo();
	HRESULT LoadingForPlay1();

private:
	HANDLE				mhThread;
	CRITICAL_SECTION	mCriticalSection;

private:
	LevelType			mNextLevelType;
	_tchar				mLoading[MAX_PATH];
	_bool				mFinished;

public:
	static CLoader* Create(LevelType _levelType);
	virtual void Free() final;
};

END