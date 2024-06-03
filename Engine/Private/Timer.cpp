#include "Timer.h"

USING(Engine)

CTimer::CTimer()
	: mTimeDelta(0.f)
{
	QueryPerformanceCounter(&mFrameTime);
	QueryPerformanceCounter(&mLastTime);
	QueryPerformanceCounter(&mFixTime);
	QueryPerformanceFrequency(&mCpuTick);
}

void CTimer::Tick()
{
	QueryPerformanceCounter(&mFrameTime);

	if (mFrameTime.QuadPart - mFixTime.QuadPart >= mCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&mCpuTick);
		mFixTime = mFrameTime;
	}

	mTimeDelta = ((mFrameTime.QuadPart) - (mLastTime.QuadPart)) / _float(mCpuTick.QuadPart);

	mLastTime = mFrameTime;
}

CTimer* CTimer::Create()
{
	auto instance = _new CTimer;
	return instance;
}

void CTimer::Free()
{
}