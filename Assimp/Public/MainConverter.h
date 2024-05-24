#pragma once
#include "Assimp_Defines.h"
#include "ModelConverter.h"

BEGIN(Assimp)

class CMainConverter final : public Engine::CBase
{
private:
	explicit CMainConverter() = default;
	virtual ~CMainConverter() = default;

public:
	HRESULT ModelConvert(_uint _type, const std::wstring& _filePath);

public:
	static void Convert(_uint _type, const std::wstring& _filePath);
	virtual void Free() override;
};

END