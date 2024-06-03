#pragma once
#include "AssimpDefines.h"

BEGIN(Assimp)

class CModelConverter final : public Engine::CBase
{
public:
	// �� Ÿ��
	enum class ModelType { NONANIM, ANIM, TYPEEND };

private:
	explicit CModelConverter();
	virtual ~CModelConverter() = default;

public:
	// Ŭ���� ���� �����Լ�
	HRESULT ConvertModel(_uint _type, const std::string& _modelFilePath);

private:
	// ������� ���� �б� ���� �Ҵ��� ����� ��ü.
	const aiScene*						mAIScene;
	// m_pAIScene�� ���� ������ �Ѱ��ֱ����� �Ҵ��� ����.
	Assimp::Importer					mImporter;

private: /* For. BoneNode */
	// ������ ������ ���� ���������̳�
	std::vector<Engine::NODE>			mNodes;

private: /* For. Converted Structs */
	// ��ȯ�Ͽ� ��Ƶ� ����ü. 
	// ������ Ŭ���̾�Ʈ, ���� ���� �ѱ������ ������ ����
	Engine::MODEL						mModel;
	// AIScene->mMeshes �� ������ ������� ����
	std::vector<Engine::MESH>			mMeshes;
	// AIScene->mMaterials �� ������ ������� ����
	std::vector<Engine::MATERIAL>		mMaterials;
	// AIScene->mAnimations �� ������ ������� ����
	std::vector<Engine::ANIMATION>		mAnimations;

private:
	HRESULT ConvertBones(aiNode* _node, _uint _parentIndex, _Inout_ _uint* _childIndex, _bool _root = false);
	// ���� �ε��� ������� ó���ϱ� ���� ���� �Լ�
	HRESULT SortBones();
	HRESULT ConvertMeshes();
	HRESULT StoreMesh(const aiMesh* _aiMesh, _Inout_ Engine::MESH* _outMesh);
	HRESULT ConvertMaterials(ModelType _type, const std::string& _modelFilePath, const std::string& _saveDirectory);
	HRESULT ConvertAnimations();

private:
	// ������� ���� ����ü�� �������� ���Ϸ� ���ִ� �Լ�
	HRESULT SaveData(ModelType _type, const std::string& _saveDirectory, const std::string& _fileName);

public:
	static HRESULT Convert(_uint _type, const std::string& _modelFilePath);
	virtual void Free() override;
};

END