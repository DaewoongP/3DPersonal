#include "Loader.h"

_uint WINAPI Thread_Main(void* _arg)
{
	using namespace Client;

	CLoader* loader = reinterpret_cast<CLoader*>(_arg);

	if (FAILED(loader->Loading()))
		return 0;

	return 0;
}

HRESULT Client::CLoader::Initialize(LevelType _levelType)
{
	mNextLevelType = _levelType;

	InitializeCriticalSection(&mCriticalSection);

	mhThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	if (0 == mhThread)
	{
		DISPLAY_ERROR(TEXT("Failed beginthreadex"));
		__debugbreak();
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT Client::CLoader::Loading()
{
	FAILED_CHECK_RETURN_MSG(CoInitializeEx(nullptr, 0), E_FAIL, TEXT("Failed"));

	EnterCriticalSection(&mCriticalSection);

	HRESULT	hr = 0;

	switch (mNextLevelType)
	{
	case LevelType::LOGO:
		hr = LoadingForLogo();
		break;
	case LevelType::PLAY1:
		hr = LoadingForPlay1();
		break;
	default:
		DISPLAY_ERROR(TEXT("Loading Func Failed"));
		__debugbreak();
		break;
	}

	LeaveCriticalSection(&mCriticalSection);

	FAILED_RETURN(hr, E_FAIL);

	return S_OK;
}

HRESULT Client::CLoader::LoadingForLogo()
{
	mFinished = true;

	return S_OK;
}

HRESULT Client::CLoader::LoadingForPlay1()
{
	mFinished = true;

	return S_OK;
}

Client::CLoader* Client::CLoader::Create(LevelType _levelType)
{
	auto instance = _new CLoader;

	NULL_CHECK_RETURN_MSG(instance, nullptr, TEXT("new instance nullptr"));
	FAILED_CHECK_RETURN_MSG(instance->Initialize(_levelType), nullptr, TEXT("Create Failed"))

	return instance;
}

void Client::CLoader::Free()
{
	WaitForSingleObject(mhThread, INFINITE);

	DeleteCriticalSection(&mCriticalSection);
	CloseHandle(mhThread);
}
