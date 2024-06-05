#include "Composite.h"

USING(Engine)

CComposite::CComposite()
    : CComponent()
{
}

CComposite::CComposite(CComposite& _rhs)
    : CComponent(_rhs)
{
}

CComponent* CComposite::GetComponent(const std::wstring& _key)
{
    return FindComponent(_key);
}

void CComposite::Tick(_float _timeDelta)
{
    for (auto& component : mComponents)
    {
        component.second->Tick(_timeDelta);
    }

    __super::Tick(_timeDelta);
}

void CComposite::LateTick(_float _timeDelta)
{
    for (auto& component : mComponents)
    {
        component.second->LateTick(_timeDelta);
    }

    __super::LateTick(_timeDelta);
}

CComponent* CComposite::AddComponent(const std::wstring& _prototypeKey, const std::wstring& _componentKey, void* _arg, _int _levelIndex)
{
    return nullptr;
}

CComponent* CComposite::FindComponent(const std::wstring& _key)
{
    auto iter = mComponents.find(_key);

    if (iter == mComponents.end())
        return nullptr;

    return iter->second;
}

void CComposite::Free()
{
}
