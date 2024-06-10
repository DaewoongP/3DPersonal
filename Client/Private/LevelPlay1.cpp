#include "LevelPlay1.h"
#include "GameInstance.h"
#include "LevelLoading.h"

HRESULT Client::CLevelPlay1::Initialize()
{
	return S_OK;
}

void Client::CLevelPlay1::Tick(_float _timeDelta)
{
	DebugFunc::Text("Hello World", _timeDelta);
}

Client::CLevelPlay1* Client::CLevelPlay1::Create()
{
	auto instance = _new CLevelPlay1;

	NULL_CHECK_RETURN_MSG(instance, nullptr, TEXT("new instance nullptr"));
	FAILED_CHECK_RETURN_MSG(instance->Initialize(), nullptr, TEXT("Create Failed"))

	return instance;
}

void Client::CLevelPlay1::Free()
{
}
