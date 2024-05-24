#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class LevelLogo : public Engine::Level
{
public:
	LevelLogo() = default;
	~LevelLogo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float _timeDelta) override;

public:
	static std::unique_ptr<LevelLogo> Create();
};

END