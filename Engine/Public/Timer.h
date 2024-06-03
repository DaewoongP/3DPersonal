#pragma once
#include "Base.h"

BEGIN(Engine)

class CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;

public:
	_float	GetTimeDelta() const { return mTimeDelta; }

public:
	void	Tick();

private:
	LARGE_INTEGER			mFrameTime;
	LARGE_INTEGER			mFixTime;
	LARGE_INTEGER			mLastTime;
	LARGE_INTEGER			mCpuTick;

	_float					mTimeDelta;

public:
	static CTimer* Create();
	virtual void Free() final;
};

END