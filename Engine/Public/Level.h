#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
public:
	explicit CLevel() = default;
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize() { return S_OK; }
	virtual void Tick(_float _timeDelta) {}
	virtual HRESULT Render() { return S_OK; }
};

END