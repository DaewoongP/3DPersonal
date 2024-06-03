#pragma once
#include "AssimpDefines.h"

BEGIN(Assimp)

class CModelConverter final : public Engine::CBase
{
public:
	// 모델 타입
	enum class ModelType { NONANIM, ANIM, TYPEEND };

private:
	explicit CModelConverter();
	virtual ~CModelConverter() = default;

public:
	// 클래스 최초 구동함수
	HRESULT ConvertModel(_uint _type, const std::string& _modelFilePath);

private:
	// 어심프로 모델을 읽기 위해 할당한 어심프 객체.
	const aiScene*						mAIScene;
	// m_pAIScene에 읽은 파일을 넘겨주기위해 할당한 변수.
	Assimp::Importer					mImporter;

private: /* For. BoneNode */
	// 생성한 뼈들을 담은 벡터컨테이너
	std::vector<Engine::NODE>			mNodes;

private: /* For. Converted Structs */
	// 변환하여 담아둘 구조체. 
	// 실제로 클라이언트, 툴에 값을 넘기기위한 값들을 정의
	Engine::MODEL						mModel;
	// AIScene->mMeshes 의 값들을 담기위한 변수
	std::vector<Engine::MESH>			mMeshes;
	// AIScene->mMaterials 의 값들을 담기위한 변수
	std::vector<Engine::MATERIAL>		mMaterials;
	// AIScene->mAnimations 의 값들을 담기위한 변수
	std::vector<Engine::ANIMATION>		mAnimations;

private:
	HRESULT ConvertBones(aiNode* _node, _uint _parentIndex, _Inout_ _uint* _childIndex, _bool _root = false);
	// 뼈를 인덱스 순서대로 처리하기 위한 정렬 함수
	HRESULT SortBones();
	HRESULT ConvertMeshes();
	HRESULT StoreMesh(const aiMesh* _aiMesh, _Inout_ Engine::MESH* _outMesh);
	HRESULT ConvertMaterials(ModelType _type, const std::string& _modelFilePath, const std::string& _saveDirectory);
	HRESULT ConvertAnimations();

private:
	// 현재까지 담은 구조체와 변수들을 파일로 써주는 함수
	HRESULT SaveData(ModelType _type, const std::string& _saveDirectory, const std::string& _fileName);

public:
	static HRESULT Convert(_uint _type, const std::string& _modelFilePath);
	virtual void Free() override;
};

END