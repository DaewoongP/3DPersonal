#include "..\Public\MainConverter.h"

USING(Assimp)

HRESULT CMainConverter::ModelConvert(_uint _type, const std::wstring& _filePath)
{
	// ���丮 ��θ� ��ȸ�� iterator
	fs::directory_iterator iter(fs::absolute(_filePath));

	while (iter != fs::end(iter))
	{
		// ���� ���丮 ��θ� ����ִ� ���� (iterator�� ����)
		const fs::directory_entry& entry = *iter;

		// ���� entry ������ ���丮���� Ȯ�� �� ���丮�̸� ���
		if (fs::is_directory(entry.path()))
		{
			ModelConvert(_type, entry.path().c_str());
		}
		else
		{
			// fbx���� üũ
			if (!lstrcmp(entry.path().extension().c_str(), TEXT(".fbx")) ||
				!lstrcmp(entry.path().extension().c_str(), TEXT(".FBX")))
			{
				std::cout << entry.path() << std::endl;

				// �ش��ϴ� ������ ��θ� ���� �� �����Ϳ� �Ҵ�
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
