#include "Loader.h"

_uint WINAPI Thread_Main(void* _arg)
{
	using namespace Client;

	Loader* loader = reinterpret_cast<Loader*>(_arg);

	if (FAILED(loader->Loading()))
		return 0;

	return 0;
}

Client::Loader::~Loader()
{
	WaitForSingleObject(mhThread, INFINITE);

	DeleteCriticalSection(&mCriticalSection);
	CloseHandle(mhThread);
}

HRESULT Client::Loader::Initialize(LevelType _levelType)
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

HRESULT Client::Loader::Loading()
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

HRESULT Client::Loader::LoadingForLogo()
{
	mFinished = true;

	return S_OK;
}

HRESULT Client::Loader::LoadingForPlay1()
{
	mFinished = true;

	return S_OK;
}

std::unique_ptr<Client::Loader> Client::Loader::Create(LevelType _levelType)
{
	auto instance = std::make_unique<Client::Loader>();
	FAILED_CHECK_RETURN_MSG(instance->Initialize(_levelType), nullptr, TEXT("Failed"));
	return instance;
}
