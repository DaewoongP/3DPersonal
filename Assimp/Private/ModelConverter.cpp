#include "..\Public\ModelConverter.h"

USING(Assimp)

CModelConverter::CModelConverter()
	: mAIScene(nullptr)
{
	ZEROMEM(&mModel);
}

HRESULT CModelConverter::ConvertModel(_uint _type, const std::string& _modelFilePath)
{
	ModelType type{ static_cast<ModelType>(_type) };
	_uint	flag{ 0 };

	if (ModelType::ANIM == type)
		flag = aiProcess_GlobalScale | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		flag = aiProcess_PreTransformVertices | aiProcess_GlobalScale | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	// importer���� ������ ����
	mAIScene = mImporter.ReadFile(_modelFilePath, flag);

	NULL_CHECK_RETURN_MSG(mAIScene, E_FAIL, TEXT("Failed Importer Read File"));

	std::cout << "Convert Bones..." << std::endl;
	FAILED_CHECK_RETURN_MSG(ConvertBones(mAIScene->mRootNode, 0, nullptr, true), E_FAIL, TEXT("Failed ConvertBones"));
	SortBones();

	std::cout << "Convert Meshes..." << std::endl;
	FAILED_CHECK_RETURN_MSG(ConvertMeshes(), E_FAIL, TEXT("Failed ConvertMeshes"));


	_char fileName[MAX_PATH]{ "" };
	std::string FullPath{ _modelFilePath };
	_splitpath_s(FullPath.c_str(), nullptr, 0, nullptr, 0, fileName, MAX_PATH, nullptr, 0);

	std::string finalPath{ "../../IGResource/Models/" };
	std::string subFileName{ fileName };

	// Create Directory
	if (ModelType::ANIM == type)
		finalPath += "Anims/" + subFileName + "/";
	else if (ModelType::NONANIM == type)
		finalPath += "NonAnims/" + subFileName + "/";

	fs::create_directory(finalPath);

	std::cout << "Convert Materials..." << std::endl;
	FAILED_CHECK_RETURN_MSG(ConvertMaterials(type, _modelFilePath, finalPath), E_FAIL, TEXT("Failed ConvertMaterials"));

	if (ModelType::ANIM == type)
	{
		// �ִϸ��̼� �����ϰ�쿡�� ����.
		std::cout << "Convert Animations..." << std::endl;
		FAILED_CHECK_RETURN_MSG(ConvertAnimations(), E_FAIL, TEXT("Failed ConvertAnimations"));
	}

	std::cout << "Save Data..." << std::endl;
	FAILED_CHECK_RETURN_MSG(SaveData(type, finalPath, fileName), E_FAIL, TEXT("Failed SaveData"));

	std::cout << "Convert Success!" << std::endl;
	std::cout << "=====================================" << std::endl;

	return S_OK;
}

HRESULT CModelConverter::ConvertBones(aiNode* _node, _uint _parentIndex, _Inout_ _uint* _childIndex, _bool _root)
{
	// ���Ͽ� ������ ��� ����ü
	Engine::NODE node;
	ZEROMEM(&node);

	// �� �̸�
	_tchar BoneName[MAX_PATH]{ TEXT("") };
	CharToWChar(_node->mName.data, BoneName);
	lstrcpy(node.Name, BoneName);

	// ����� ��������� �޾ƿͼ� ��ġ ��Ŵ (Col Major -> Row Major)
	// �⺻������ assimp ���̺귯���� Col Major ��ķ� ����ϱ� ����.
	_float4x4 TransposeMatrix;
	memcpy(&TransposeMatrix, &_node->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransposeMatrix)));
	memcpy(&node.Transformation, &TransposeMatrix, sizeof(_float4x4));

	static _uint currentNodeIndex{ 0 };
	// Root Check
	if (true == _root)
	{
		// Parent except
		node.Parent = -1;
	}
	else
	{
		// �ڽ� �ε��� �Ҵ�
		*_childIndex = ++currentNodeIndex;

		node.Parent = _parentIndex;
	}

	// ����ü�� �ε���, �ڽ� ���� �� ����.
	node.NodeIndex = currentNodeIndex;
	node.NumChildren = _node->mNumChildren;

	// �ڽ��� ���� ��� �ڽĺ����� �����Ҵ�.
	if (0 != _node->mNumChildren)
	{
		node.Children = new _uint[_node->mNumChildren];
		ZeroMemory(node.Children, sizeof(_uint) * _node->mNumChildren);
	}

	// �ڽ� ������ŭ ��ȸ�ϸ� ����Լ� ȣ���Ͽ� ���� ��Ӱ��� ó��.
	for (_uint i = 0; i < _node->mNumChildren; ++i)
	{
		_uint retIndex{ 0 };
		ConvertBones(_node->mChildren[i], node.NodeIndex, &retIndex);
		node.Children[i] = retIndex;
	}

	// ������ ���������̳ʿ� ����.
	mNodes.push_back(node);

	return S_OK;
}

HRESULT CModelConverter::SortBones()
{
	std::sort(mNodes.begin(), mNodes.end(), [](Engine::NODE& _sour, Engine::NODE& _dest) {

		if (_sour.NodeIndex < _dest.NodeIndex)
			return true;
		else
			return false;
		});

	// �𵨿� ��� ���� ����.
	mModel.NumNodes = static_cast<_uint>(mNodes.size());

	return S_OK;
}

HRESULT CModelConverter::ConvertMeshes()
{
	// �޽����� ����ü�� �� ����
	mModel.NumMeshes = mAIScene->mNumMeshes;

	// �޽� ������ŭ ��ȸ�ϸ� �޽��� �ش��ϴ� ������ �������
	for (_uint i = 0; i < mAIScene->mNumMeshes; ++i)
	{
		Engine::MESH mesh;
		ZEROMEM(&mesh);

		// �޽��� ������ ���·� �Ѱ��־� ������ ��� �Ҵ� �Ͽ� ä�� ����.
		FAILED_CHECK_RETURN_MSG(StoreMesh(mAIScene->mMeshes[i], &mesh), E_FAIL, TEXT("Failed Store Mesh"));
		
		// �޽� ����
		mMeshes.emplace_back(mesh);
	}

	return S_OK;
}

HRESULT CModelConverter::StoreMesh(const aiMesh* _aiMesh, _Inout_ Engine::MESH* _outMesh)
{
	// ���׸��� �ε��� ���� ����
	_outMesh->MaterialIndex = _aiMesh->mMaterialIndex;

	// �޽� �̸�
	_tchar MeshName[MAX_PATH]{ TEXT("") };
	CharToWChar(_aiMesh->mName.data, MeshName);
	lstrcpy(_outMesh->Name, MeshName);

	// ���� ������ ����
	_outMesh->NumVertices = _aiMesh->mNumVertices;
	_outMesh->NumFaces = _aiMesh->mNumFaces;

	// �޽� ǥ�� �����Ҵ��Ͽ� ó��
	_outMesh->Faces = new Engine::FACE[_outMesh->NumFaces];
	ZeroMemory(_outMesh->Faces, sizeof(Engine::FACE) * _outMesh->NumFaces);

	// �޽� ǥ���� ��ȸ�ϸ� �޽��� �ε����� ����
	for (_uint i = 0; i < _aiMesh->mNumFaces; ++i)
	{
		_outMesh->Faces[i].NumIndices = _aiMesh->mFaces[i].mNumIndices;

		_outMesh->Faces[i].Indices = new _uint[_outMesh->Faces[i].NumIndices];
		ZeroMemory(_outMesh->Faces[i].Indices, sizeof(_uint) * _outMesh->Faces[i].NumIndices);

		for (_uint j = 0; j < _outMesh->Faces[i].NumIndices; ++j)
		{
			_outMesh->Faces[i].Indices[j] = _aiMesh->mFaces[i].mIndices[j];
		}
	}

	// ���ؽ��� ����� ���� ������, �븻, UV��ǥ, ź��Ʈ�� ����� ���� �����Ҵ�.
	_outMesh->Positions = new _float3[_aiMesh->mNumVertices];
	ZeroMemory(_outMesh->Positions, sizeof(_float3) * _aiMesh->mNumVertices);

	_outMesh->Normals = new _float3[_aiMesh->mNumVertices];
	ZeroMemory(_outMesh->Normals, sizeof(_float3) * _aiMesh->mNumVertices);

	_outMesh->TexCoords = new _float2[_aiMesh->mNumVertices];
	ZeroMemory(_outMesh->TexCoords, sizeof(_float2) * _aiMesh->mNumVertices);

	_outMesh->Tangents = new _float3[_aiMesh->mNumVertices];
	ZeroMemory(_outMesh->Tangents, sizeof(_float3) * _aiMesh->mNumVertices);

	// ���� ���ؽ� ��ȸ�ϸ� ��� ���� ����.
	for (_uint i = 0; i < _aiMesh->mNumVertices; i++)
	{
		memcpy(&_outMesh->Positions[i], &_aiMesh->mVertices[i], sizeof(_float3));
		memcpy(&_outMesh->Normals[i], &_aiMesh->mNormals[i], sizeof(_float3));
		memcpy(&_outMesh->TexCoords[i], &_aiMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&_outMesh->Tangents[i], &_aiMesh->mTangents[i], sizeof(_float3));
	}

	// �޽��� ���� ���� ����
	_outMesh->NumBones = _aiMesh->mNumBones;

	// �� ��ü �����Ҵ�
	_outMesh->Bones = new Engine::BONE[_aiMesh->mNumBones];
	ZeroMemory(_outMesh->Bones, sizeof(Engine::BONE) * _aiMesh->mNumBones);

	// �޽��� ������ ��ȸ�ϸ� ���� ������ ������
	for (_uint i = 0; i < _aiMesh->mNumBones; i++)
	{
		aiBone* aiBone{ _aiMesh->mBones[i] };

		Engine::BONE& bone{ _outMesh->Bones[i] };
		ZEROMEM(&bone);
		// �ش��ϴ� ���� �̸�
		_tchar boneName[MAX_PATH]{ TEXT("") };
		CharToWChar(aiBone->mName.data, boneName);
		lstrcpy(bone.Name, boneName);

		// assimp ���̺귯���� �⺻������ ColMajor �̹Ƿ� Row Major ���·� ��ġ�Ͽ� ����.
		_float4x4 TransposeMatrix;
		memcpy(&TransposeMatrix, &aiBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransposeMatrix)));
		memcpy(&bone.OffsetMatrix, &TransposeMatrix, sizeof _float4x4);

		// �޽� ���� weight��
		bone.NumWeights = aiBone->mNumWeights;

		bone.Weights = new Engine::WEIGHT[aiBone->mNumWeights];
		ZeroMemory(bone.Weights, sizeof(Engine::WEIGHT) * aiBone->mNumWeights);

		// �����Ҵ��� weight�� ��ȸ�ϸ� ���ؽ� ���̵�� weight ���� ����.
		for (_uint j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight	aiVertexWeight = { aiBone->mWeights[j] };

			bone.Weights[j].VertexId = aiVertexWeight.mVertexId;
			bone.Weights[j].Weight = aiVertexWeight.mWeight;
		}
	}

	return S_OK;
}

HRESULT CModelConverter::ConvertMaterials(ModelType _type, const std::string& _modelFilePath, const std::string& _saveDirectory)
{
	// ���׸��� ����
	mModel.NumMaterials = mAIScene->mNumMaterials;

	// ���� ���׸��� ��ȸ
	for (_uint i = 0; i < mAIScene->mNumMaterials; ++i)
	{
		Engine::MATERIAL material;
		ZEROMEM(&material);

		// �ش��ϴ� ���׸����� �ؽ�ó ��� ��ȸ�ϸ� ����
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString	aistrPath;

			if (FAILED(mAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &aistrPath)))
				continue;

			std::string texturePath{ aistrPath.data };

			_char makeDrivePath[MAX_PATH]{ "" };
			_char makeModelPath[MAX_PATH]{ "" };
			_splitpath_s(_modelFilePath.c_str(), makeDrivePath, MAX_PATH, makeModelPath, MAX_PATH, nullptr, 0, nullptr, 0);

			std::string drivePath{ makeDrivePath };
			std::string modelDirPath{ makeModelPath };

			// �������ø� ����� ��� �ڿ� ���� �ٴ� ���ڿ��� ������ ���� �� �־� ������ ���·� ����
			size_t offset{ std::string::npos };
			while (std::string::npos != (offset = modelDirPath.find("\\")))
			{
				modelDirPath.replace(offset, 1, "/");
			}

			while (std::string::npos != (offset = texturePath.find("\\")))
			{
				texturePath.replace(offset, 1, "/");
			}

			while (std::string::npos != texturePath.find("../"))
			{
				size_t DirSlashIndex = modelDirPath.rfind("/");

				modelDirPath = modelDirPath.substr(0, DirSlashIndex);

				DirSlashIndex = modelDirPath.rfind("/");

				modelDirPath = modelDirPath.substr(0, DirSlashIndex + 1);

				texturePath = texturePath.substr(3);
			}

			// ������ ��ε� strcat
			std::string fullPath = drivePath + modelDirPath + texturePath;

			std::error_code ec;
			// �ؽ�ó ���� ���� �� ����
			// skip_existing, overwrite_existing, update_existing 
			// ���� ������ �����Ұ�� ������ ���Ƿ� �ɼ� �����.
			fs::copy(fullPath, _saveDirectory, fs::copy_options::skip_existing, ec);

			// �ؽ�ó Ÿ�� ����
			_char fileName[MAX_PATH]{ "" };
			_char fileExt[MAX_PATH]{ "" };
			_splitpath_s(fullPath.c_str(), nullptr, 0, nullptr, 0, fileName, MAX_PATH, fileExt, MAX_PATH);

			strcat_s(fileName, fileExt);
			std::string tempPath{ _saveDirectory + fileName };
			_tchar	convertedTexPath[MAX_PATH]{ TEXT("") };
			CharToWChar(tempPath.c_str(), convertedTexPath);

			lstrcpy(material.MaterialTexture[j].TexPath, convertedTexPath);
			material.MaterialTexture[j].TexType = static_cast<Engine::TextureType>(j);
		}

		// ���׸��� ����
		mMaterials.emplace_back(material);
	}

	return S_OK;
}

HRESULT CModelConverter::ConvertAnimations()
{
	mModel.NumAnimations = mAIScene->mNumAnimations;

	// �ִϸ��̼� ��ȸ
	for (_uint i = 0; i < mAIScene->mNumAnimations; ++i)
	{
		aiAnimation* aiAnimation{ mAIScene->mAnimations[i] };

		Engine::ANIMATION animation;
		ZEROMEM(&animation);
		// �ִϸ��̼� �̸�
		_tchar animName[MAX_PATH] = TEXT("");
		CharToWChar(aiAnimation->mName.data, animName);
		lstrcpy(animation.Name, animName);

		// �ִϸ��̼� ���� ����
		animation.Duration = static_cast<_float>(aiAnimation->mDuration);
		animation.TickPerSecond = static_cast<_float>(aiAnimation->mTicksPerSecond);
		animation.NumChannels = aiAnimation->mNumChannels;

		animation.Channels = new Engine::CHANNEL[aiAnimation->mNumChannels];
		ZeroMemory(animation.Channels, sizeof(Engine::CHANNEL) * aiAnimation->mNumChannels);

		// �ִϸ��̼� ä�� ��ȸ
		for (_uint j = 0; j < aiAnimation->mNumChannels; ++j)
		{
			aiNodeAnim* aiChannel = aiAnimation->mChannels[j];

			Engine::CHANNEL channel;
			ZEROMEM(&channel);
			// ä�� �̸�
			_tchar channelName[MAX_PATH] = TEXT("");
			CharToWChar(aiChannel->mNodeName.data, channelName);
			lstrcpy(channel.Name, channelName);

			// ä���� ������ Ű ����
			channel.NumScalingKeys = aiChannel->mNumScalingKeys;
			channel.ScalingKeys = new Engine::VECTORKEY[aiChannel->mNumScalingKeys];
			ZeroMemory(channel.ScalingKeys, sizeof(Engine::VECTORKEY) * aiChannel->mNumScalingKeys);

			for (_uint k = 0; k < aiChannel->mNumScalingKeys; ++k)
			{
				channel.ScalingKeys[k].Time = _float(aiChannel->mScalingKeys[k].mTime);
				memcpy(&channel.ScalingKeys[k].Value, &aiChannel->mScalingKeys[k].mValue, sizeof(_float3));
			}

			// ä���� �����̼� Ű ���� (���ʹϾ� ����)
			channel.NumRotationKeys = aiChannel->mNumRotationKeys;
			channel.RotationKeys = new Engine::QUATERNIONKEY[aiChannel->mNumRotationKeys];
			ZeroMemory(channel.RotationKeys, sizeof(Engine::QUATERNIONKEY) * aiChannel->mNumRotationKeys);

			for (_uint k = 0; k < aiChannel->mNumRotationKeys; ++k)
			{
				channel.RotationKeys[k].Time = static_cast<_float>(aiChannel->mRotationKeys[k].mTime);

				memcpy(&channel.RotationKeys[k].Value, &aiChannel->mRotationKeys[k].mValue, sizeof(_float4));
			}

			// ä���� ������ Ű ����
			channel.NumPositionKeys = aiChannel->mNumPositionKeys;
			channel.PositionKeys = new Engine::VECTORKEY[aiChannel->mNumPositionKeys];
			ZeroMemory(channel.PositionKeys, sizeof(Engine::VECTORKEY) * aiChannel->mNumPositionKeys);

			for (_uint k = 0; k < aiChannel->mNumPositionKeys; ++k)
			{
				channel.PositionKeys[k].Time = static_cast<_float>(aiChannel->mPositionKeys[k].mTime);
				memcpy(&channel.PositionKeys[k].Value, &aiChannel->mPositionKeys[k].mValue, sizeof(_float3));
			}

			// ä�� ����
			animation.Channels[j] = channel;
		}

		// �ִϸ��̼� ����
		mAnimations.emplace_back(animation);
	}

	return S_OK;
}

HRESULT CModelConverter::SaveData(ModelType _type, const std::string& _saveDirectory, const std::string& _fileName)
{
	std::string name{ _saveDirectory + _fileName + ".dat" };

	HANDLE file = CreateFileA(name.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == file)
		return E_FAIL;

	_ulong	byte{ 0 };
	_ulong	strByte{ 0 };

	// Write Nodes

	// Nodes NumNodes
	WriteFile(file, &(mModel.NumNodes), sizeof(_uint), &byte, nullptr);

	for (auto& Node : mNodes)
	{
		// Node Name
		strByte = sizeof(_tchar) * (lstrlen(Node.Name) + 1);
		WriteFile(file, &strByte, sizeof(_ulong), &byte, nullptr);
		WriteFile(file, Node.Name, strByte, &byte, nullptr);

		// Node Transformation
		WriteFile(file, &(Node.Transformation), sizeof(_float4x4), &byte, nullptr);

		// Node NodeIndex
		WriteFile(file, &(Node.NodeIndex), sizeof(_uint), &byte, nullptr);

		// Node Parent
		WriteFile(file, &(Node.Parent), sizeof(_int), &byte, nullptr);

		// Node NumChildren
		WriteFile(file, &(Node.NumChildren), sizeof(_uint), &byte, nullptr);

		// Node Children (array)
		WriteFile(file, Node.Children, sizeof(_uint) * Node.NumChildren, &byte, nullptr);
	}

	// Write Meshes

	// Meshes NumMeshes
	WriteFile(file, &(mModel.NumMeshes), sizeof(_uint), &byte, nullptr);

	for (_uint i = 0; i < mModel.NumMeshes; ++i)
	{
		Engine::MESH mesh = mMeshes[i];

		// Mesh Name
		strByte = sizeof(_tchar) * (lstrlen(mesh.Name) + 1);
		WriteFile(file, &strByte, sizeof(_ulong), &byte, nullptr);
		WriteFile(file, mesh.Name, strByte, &byte, nullptr);

		// Mesh MaterialIndex
		WriteFile(file, &(mesh.MaterialIndex), sizeof(_uint), &byte, nullptr);

		// Mesh NumVertices
		WriteFile(file, &(mesh.NumVertices), sizeof(_uint), &byte, nullptr);

		// Mesh NumFaces
		WriteFile(file, &(mesh.NumFaces), sizeof(_uint), &byte, nullptr);

		for (_uint j = 0; j < mesh.NumFaces; ++j)
		{
			Engine::FACE face = mesh.Faces[j];
			// Face NumIndices
			WriteFile(file, &(face.NumIndices), sizeof(_uint), &byte, nullptr);

			// Face Indices
			WriteFile(file, face.Indices, sizeof(_uint) * face.NumIndices, &byte, nullptr);
		}

		// Mesh Positions
		WriteFile(file, mesh.Positions, sizeof(_float3) * mesh.NumVertices, &byte, nullptr);

		// Mesh Normals
		WriteFile(file, mesh.Normals, sizeof(_float3) * mesh.NumVertices, &byte, nullptr);

		// Mesh TexCoords
		WriteFile(file, mesh.TexCoords, sizeof(_float2) * mesh.NumVertices, &byte, nullptr);

		// Mesh Tangents
		WriteFile(file, mesh.Tangents, sizeof(_float3) * mesh.NumVertices, &byte, nullptr);

		// Mesh NumBones
		WriteFile(file, &(mesh.NumBones), sizeof(_uint), &byte, nullptr);

		// Write Bones
		for (_uint j = 0; j < mesh.NumBones; j++)
		{
			Engine::BONE bone = mesh.Bones[j];

			// Bone Name
			strByte = (_ulong)sizeof(_tchar) * (lstrlen(bone.Name) + 1);
			WriteFile(file, &strByte, sizeof(_ulong), &byte, nullptr);
			WriteFile(file, bone.Name, strByte, &byte, nullptr);

			// Mesh OffsetMatrix
			WriteFile(file, &(bone.OffsetMatrix), sizeof(_float4x4), &byte, nullptr);

			// Mesh NumWeights
			WriteFile(file, &(bone.NumWeights), sizeof(_uint), &byte, nullptr);

			// Write Weights
			for (_uint k = 0; k < bone.NumWeights; k++)
			{
				Engine::WEIGHT weight = bone.Weights[k];

				// Weight VertexId
				WriteFile(file, &(weight.VertexId), sizeof(_uint), &byte, nullptr);

				// Weight Weight
				WriteFile(file, &(weight.Weight), sizeof(_float), &byte, nullptr);
			}
		}
	}

	// Write Materials

	// Material NumMaterials
	WriteFile(file, &(mModel.NumMaterials), sizeof(_uint), &byte, nullptr);

	for (_uint i = 0; i < mModel.NumMaterials; ++i)
	{
		Engine::MATERIAL material = mMaterials[i];

		// MaterialTex
		WriteFile(file, material.MaterialTexture, sizeof(Engine::MATERIALTEX) * TextureType_MAX, &byte, nullptr);
	}

	// Write Animations
	if (ModelType::ANIM == _type)
	{
		// Animation NumAnimations
		WriteFile(file, &(mModel.NumAnimations), sizeof(_uint), &byte, nullptr);

		for (_uint i = 0; i < mModel.NumAnimations; ++i)
		{
			Engine::ANIMATION animation = mAnimations[i];

			// Animation Name
			strByte = (_ulong)sizeof(_tchar) * (lstrlen(animation.Name) + 1);
			WriteFile(file, &strByte, sizeof(_ulong), &byte, nullptr);
			WriteFile(file, animation.Name, strByte, &byte, nullptr);

			// Animation Duration
			WriteFile(file, &(animation.Duration), sizeof(_float), &byte, nullptr);

			// Animation TickPerSecond
			WriteFile(file, &(animation.TickPerSecond), sizeof(_float), &byte, nullptr);

			// Animation NumChannels
			WriteFile(file, &(animation.NumChannels), sizeof(_uint), &byte, nullptr);

			for (_uint j = 0; j < animation.NumChannels; ++j)
			{
				Engine::CHANNEL channel = animation.Channels[j];

				// Channel Name
				strByte = (_ulong)sizeof(_tchar) * (lstrlen(channel.Name) + 1);
				WriteFile(file, &strByte, sizeof(_ulong), &byte, nullptr);
				WriteFile(file, channel.Name, strByte, &byte, nullptr);

				// Channel NumScalingKeys
				WriteFile(file, &(channel.NumScalingKeys), sizeof(_uint), &byte, nullptr);

				// Channel ScalingKeys
				WriteFile(file, channel.ScalingKeys, sizeof(Engine::VECTORKEY) * (channel.NumScalingKeys), &byte, nullptr);

				// Channel NumRotationKeys
				WriteFile(file, &(channel.NumRotationKeys), sizeof(_uint), &byte, nullptr);

				// Channel RotationKeys
				WriteFile(file, channel.RotationKeys, sizeof(Engine::QUATERNIONKEY) * (channel.NumRotationKeys), &byte, nullptr);

				// Channel NumPositionKeys
				WriteFile(file, &(channel.NumPositionKeys), sizeof(_uint), &byte, nullptr);

				// Channel PositionKeys
				WriteFile(file, channel.PositionKeys, sizeof(Engine::VECTORKEY) * (channel.NumPositionKeys), &byte, nullptr);
			}
		}
	}

	CloseHandle(file);

	return S_OK;
}

HRESULT CModelConverter::Convert(_uint _type, const std::string& _modelFilePath)
{
	CModelConverter* pInstance = new CModelConverter();

	if (FAILED(pInstance->ConvertModel(_type, _modelFilePath)))
	{
		MSG_BOX("Failed to Created CModelConverter");
		Utility::SafeRelease(pInstance);
		return E_FAIL;
	}
	else
	{
		Utility::SafeRelease(pInstance);
		return S_OK;
	}
}

void CModelConverter::Free()
{
	for (auto& node : mNodes)
	{
		Utility::SafeDeleteArray(node.Children);
	}

	for (auto& mesh : mMeshes)
	{
		for (_uint i = 0; i < mesh.NumFaces; ++i)
		{
			Utility::SafeDeleteArray(mesh.Faces[i].Indices);
		}
		Utility::SafeDeleteArray(mesh.Faces);

		Utility::SafeDeleteArray(mesh.Positions);
		Utility::SafeDeleteArray(mesh.Normals);
		Utility::SafeDeleteArray(mesh.TexCoords);
		Utility::SafeDeleteArray(mesh.Tangents);

		for (_uint i = 0; i < mesh.NumBones; ++i)
		{
			Utility::SafeDeleteArray(mesh.Bones[i].Weights);
		}
		Utility::SafeDeleteArray(mesh.Bones);
	}

	for (auto& animation : mAnimations)
	{
		for (_uint i = 0; i < animation.NumChannels; ++i)
		{
			Engine::CHANNEL& channel{ animation.Channels[i] };

			Utility::SafeDeleteArray(channel.ScalingKeys);
			Utility::SafeDeleteArray(channel.RotationKeys);
			Utility::SafeDeleteArray(channel.PositionKeys);
		}
		Utility::SafeDeleteArray(animation.Channels);
	}
}
