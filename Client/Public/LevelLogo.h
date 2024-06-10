#pragma once
#include "Level.h"
#include "ClientDefines.h"

BEGIN(Client)

class CLevelLogo : public Engine::CLevel
{
public:
	explicit CLevelLogo() = default;
	virtual ~CLevelLogo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float _timeDelta) override;

public:
	static CLevelLogo* Create();
	virtual void Free() final;
};

END