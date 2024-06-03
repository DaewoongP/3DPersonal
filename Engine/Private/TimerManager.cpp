#include "TimerManager.h"
#include "Timer.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimerManager)

_float CTimerManager::GetTimeDelta(const std::wstring& _timerTag)
{
	CTimer* timer = FindTimer(_timerTag);
	if (nullptr == timer)
		return 0.0;

	return timer->GetTimeDelta();
}

void CTimerManager::TickTimeDelta(const std::wstring& _timerTag)
{
	CTimer* timer = FindTimer(_timerTag);
	if (nullptr == timer)
		return;

	timer->Tick();
}

HRESULT CTimerManager::AddTimer(const std::wstring& _timerTag)
{
	CTimer* timer = FindTimer(_timerTag);

	if (nullptr != timer)
	{
		MSG_BOX("Already Have TimerTag");
		__debugbreak();
		return E_FAIL;
	}

	mTimers.emplace(_timerTag, CTimer::Create());

	return S_OK;
}

void CTimerManager::Invoke(std::function<void(void*)> _func, void* _arg, _float _callTime, _bool _loop)
{
	FUNCTIMER funcTimer;
	funcTimer.func = _func;
	funcTimer.arg = _arg;
	funcTimer.callTime = _callTime;
	funcTimer.loop = _loop;
	funcTimer.timeAcc = 0.f;

	mFunctionTimers.emplace_back(funcTimer);
}

void CTimerManager::Tick(_float _timeDelta)
{
	for (auto iter = mFunctionTimers.begin(); iter != mFunctionTimers.end();)
	{
		iter->timeAcc += _timeDelta;

		if (iter->timeAcc >= iter->callTime)
		{
			iter->func(iter->arg);
			iter->timeAcc = 0.f;

			if (true == iter->loop)
				++iter;
			else
				iter = mFunctionTimers.erase(iter);
		}
		else
			++iter;
	}
}

CTimer* CTimerManager::FindTimer(const std::wstring& _timerTag)
{
	auto iter = mTimers.find(_timerTag);

	if (iter == mTimers.end())
		return nullptr;

	return iter->second;
}

void CTimerManager::Free()
{
	for (auto& pair : mTimers)
	{
		Utility::SafeRelease(pair.second);
	}

	mTimers.clear();
}
