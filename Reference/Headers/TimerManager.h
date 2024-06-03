#pragma once
#include "Base.h"

BEGIN(Engine)

class CTimerManager : public CBase
{
	DECLARE_SINGLETON(CTimerManager)
public:
	typedef struct tagFuncTimer
	{
		_float timeAcc;
		_float callTime;
		std::function<void(void*)> func;
		void* arg;
		_bool loop;
	}FUNCTIMER;

private:
	explicit CTimerManager() = default;
	virtual ~CTimerManager() = default;

public:
	_float		GetTimeDelta(const std::wstring& _timerTag);
	void		TickTimeDelta(const std::wstring& _timerTag);

public:
	HRESULT		AddTimer(const std::wstring& _timerTag);
	void		Invoke(std::function<void(void*)> _func, void* _arg, _float _callTime, _bool _loop = false);
	void		Tick(_float _timeDelta);

private:
	class CTimer*	FindTimer(const std::wstring& _timerTag);

private:
	_umap<std::wstring, class CTimer*>	mTimers;
	std::vector<FUNCTIMER>				mFunctionTimers;

public:
	virtual void Free() final;
};

END

#define TIMER		GET_SINGLE(Engine::CTimerManager)