#include "Component.h"

USING(Engine)

CComponent::CComponent()
	: mCloned(false)
{
}

CComponent::CComponent(const CComponent& _rhs)
	: mCloned(true)
{
}
