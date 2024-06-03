#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#define MAX_NAME_LENGTH 256

namespace Engine
{
	typedef struct tagNodeDesc
	{
		wchar_t				Name[MAX_NAME_LENGTH];
		XMFLOAT4X4			Transformation; // row major

		unsigned int		NodeIndex;
		int					Parent; // Root Index = -1
		unsigned int		NumChildren;
		unsigned int*		Children; // array
	}NODE;

	typedef struct tagWeightDesc
	{
		unsigned int		VertexId;
		float				Weight;
	}WEIGHT;

	typedef struct tagBoneDesc
	{
		wchar_t				Name[MAX_NAME_LENGTH];
		XMFLOAT4X4			OffsetMatrix; // row major

		// Weight
		unsigned int		NumWeights;
		WEIGHT*				Weights; // array
	}BONE;

	typedef struct tagFaceDesc
	{
		unsigned int		NumIndices;
		unsigned int*		Indices; // array
	}FACE;

	typedef struct tagMeshDesc
	{
		wchar_t				Name[MAX_NAME_LENGTH];
		unsigned int		MaterialIndex;
		unsigned int		NumVertices;
		unsigned int		NumFaces;
		FACE*				Faces; // array
		XMFLOAT3*			Positions; // array
		XMFLOAT3*			Normals; // array
		XMFLOAT2*			TexCoords; // array
		XMFLOAT3*			Tangents; // array

		// Mesh_Bone
		unsigned int		NumBones;
		BONE* Bones; // array
	}MESH;

	enum class TextureType
	{
		NONE,
		DIFFUSE,
		SPECULAR,
		AMBIENT,
		EMISSIVE,
		HEIGHT,
		NORMALS,
		SHININESS,
		OPACITY,
		DISPLACEMENT,
		LIGHTMAP,
		REFLECTION,
		BASE_COLOR,
		NORMAL_CAMERA,
		EMISSION_COLOR,
		METALNESS,
		DIFFUSE_ROUGHNESS,
		AMBIENT_OCCLUSION,
		SHEEN,
		CLEARCOAT,
		TRANSMISSION,
		UNKNOWN,
	};
#define TextureType_MAX			static_cast<unsigned int>(Engine::TextureType::UNKNOWN)

	typedef struct tagTextureDesc
	{
		TextureType			TexType;
		wchar_t				TexPath[MAX_NAME_LENGTH];
	}MATERIALTEX;

	typedef struct tagMaterialDesc
	{
		MATERIALTEX			MaterialTexture[TextureType_MAX];
	}MATERIAL;

	typedef struct tagVectorKeyDesc
	{
		double				Time;
		XMFLOAT3			Value;
	}VECTORKEY;

	typedef struct tagQuaternionKeyDesc
	{
		double				Time;
		XMFLOAT4			Value;
	}QUATERNIONKEY;

	typedef struct tagChannelDesc
	{
		// Same as Node Name
		wchar_t				Name[MAX_NAME_LENGTH];

		// Scale
		unsigned int		NumScalingKeys;
		VECTORKEY*			ScalingKeys; // array

		// Rotation
		unsigned int		NumRotationKeys;
		QUATERNIONKEY*		RotationKeys; // array

		// Position
		unsigned int		NumPositionKeys;
		VECTORKEY*			PositionKeys;
	}CHANNEL;

	typedef struct tagAnimationDesc
	{
		wchar_t				Name[MAX_NAME_LENGTH];
		double				Duration;
		double				TickPerSecond;

		// Channel
		unsigned int		NumChannels;
		CHANNEL*			Channels; // array
	}ANIMATION;

	typedef struct tagModelDesc
	{
		// Node
		unsigned int		NumNodes;

		// Mesh
		unsigned int		NumMeshes;

		// Material
		unsigned int		NumMaterials;

		// Animation
		unsigned int		NumAnimations;
	}MODEL;
}