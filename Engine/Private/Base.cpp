#include "..\Public\Base.h"

USING(Engine)

CBase::CBase()
	: mRefCnt(1)
{
}

_uint CBase::AddRef()
{
	return ++mRefCnt;
}

_uint CBase::Release()
{
	if (0 == --mRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return mRefCnt;
}

