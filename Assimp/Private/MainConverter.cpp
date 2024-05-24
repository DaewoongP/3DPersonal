#include "..\Public\MainConverter.h"

USING(Assimp)

HRESULT CMainConverter::ModelConvert(_uint _type, const std::wstring& _filePath)
{
	// 디렉토리 경로를 순회할 iterator
	fs::directory_iterator iter(fs::absolute(_filePath));

	while (iter != fs::end(iter))
	{
		// 실제 디렉토리 경로를 담고있는 변수 (iterator의 원본)
		const fs::directory_entry& entry = *iter;

		// 현재 entry 변수가 디렉토리인지 확인 후 디렉토리이면 재귀
		if (fs::is_directory(entry.path()))
		{
			ModelConvert(_type, entry.path().c_str());
		}
		else
		{
			// fbx파일 체크
			if (!lstrcmp(entry.path().extension().c_str(), TEXT(".fbx")) ||
				!lstrcmp(entry.path().extension().c_str(), TEXT(".FBX")))
			{
				std::cout << entry.path() << std::endl;

				// 해당하는 파일의 경로를 실제 모델 컨버터에 할당
				CModelConverter::Convert(_type, entry.path().string().c_str());
			}
		}

		iter++;
	}

	return S_OK;
}


void CMainConverter::Convert(_uint _type, const std::wstring& _filePath)
{
	CMainConverter* pInstance = new CMainConverter();

	if (FAILED(pInstance->ModelConvert(_type, _filePath)))
	{
		MSG_BOX("Failed Convert Models");
	}
	else
	{
		MSG_BOX("Success Convert Models!");
	}

	Utility::SafeRelease(pInstance);
}

void CMainConverter::Free()
{
}
