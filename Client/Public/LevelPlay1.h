#pragma once
#include "Level.h"
#include "ClientDefines.h"

BEGIN(Client)

class CLevelPlay1 : public Engine::CLevel
{
public:
	explicit CLevelPlay1() = default;
	virtual ~CLevelPlay1() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float _timeDelta) override;

public:
	static CLevelPlay1* Create();
	virtual void Free() final;
};

END