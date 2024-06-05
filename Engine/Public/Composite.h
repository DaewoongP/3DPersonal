#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComposite : public CComponent
{
protected:
	explicit CComposite();
	explicit CComposite(CComposite& _rhs);
	virtual ~CComposite() = default;

public:
	CComponent* GetComponent(const std::wstring& _key);

public:
	virtual void Tick(_float _timeDelta) override;
	virtual void LateTick(_float _timeDelta) override;

public:
	CComponent* AddComponent(const std::wstring& _prototypeKey, const std::wstring& _componentKey, void* _arg = nullptr, _int _levelIndex = -1);

private:
	CComponent* FindComponent(const std::wstring& _key);

private:
	_umap<std::wstring, CComponent*>		mComponents;

public:
	virtual void Free() override;
};

END