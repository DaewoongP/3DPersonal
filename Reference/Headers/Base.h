#pragma once
#include "EngineDefines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;

public:
	_uint AddRef();
	_uint Release();

protected:
	_uint			mRefCnt;

public:
	virtual void Free() PURE;
};

END