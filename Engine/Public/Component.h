#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& _rhs);
	virtual ~CComponent() = default;

public:
	_bool IsCloned() const { return mCloned; }

public:
	virtual HRESULT Initialize(void* _arg) { return S_OK; }
	virtual void Tick(_float _timeDelta) {}
	virtual void LateTick(_float _timeDelta) {}
	virtual HRESULT Render() { return S_OK; }

private:
	_bool	mCloned;

public:
	virtual CComponent Clone(void* _arg) PURE;
};

END