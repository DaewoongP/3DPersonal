#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"

BEGIN(Client)

class Loader
{
public:
	Loader() = default;
	~Loader();
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
	static std::unique_ptr<Loader> Create(LevelType _levelType);
};

END