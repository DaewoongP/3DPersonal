#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WM_FULL, WM_WIN, WM_END };
		HWND			Hwnd;
		unsigned int	ViewportSizeX, ViewportSizeY;
		WINMODE			WinMode;
	}GRAPHICDESC;

	typedef struct tagVertex_Position
	{
		XMFLOAT3		Position;		
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int				NumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXPOS_DECL;
		

	typedef struct tagVertex_Position_TexCoord
	{
		XMFLOAT3		Position;
		XMFLOAT2		TexCoord;
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_TexCoord_Declaration
	{
		static const unsigned int				NumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXPOSTEX_DECL;


	typedef struct tagVertex_Cube
	{
		XMFLOAT3		Position;
		XMFLOAT3		TexCoord;
	}VTXCUBE;

	typedef struct ENGINE_DLL tagVertex_Cube_Declaration
	{
		static const unsigned int				NumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXCUBE_DECL;


	typedef struct tagVertex_Position_Size
	{
		XMFLOAT3		Position;
		XMFLOAT2		PSize;
	}VTXPOINT;


	typedef struct tagVertex_Position_Normal_TexCoord
	{
		XMFLOAT3		Position;
		XMFLOAT3		Normal;
		XMFLOAT2		TexCoord;
	}VTXPOSNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_TexCoord_Declaration
	{
		static const unsigned int				NumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXPOSNORTEX_DECL;

	typedef struct tagVertex_Mesh
	{
		XMFLOAT3		Position;
		XMFLOAT3		Normal;
		XMFLOAT2		TexCoord;
		XMFLOAT3		Tangent;
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertex_Mesh_Declaration
	{
		static const unsigned int				NumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXMESH_DECL;


	typedef struct tagVertex_AnimMesh
	{
		XMFLOAT3		Position;
		XMFLOAT3		Normal;
		XMFLOAT2		TexCoord;
		XMFLOAT3		Tangent;
		XMUINT4			BlendIndices;
		XMFLOAT4		BlendWeights;
	}VTXANIMMESH;

	typedef struct ENGINE_DLL tagVertex_AnimMesh_Declaration
	{
		static const unsigned int				NumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXANIMMESH_DECL;

	typedef struct tagVertex_Instance
	{		
		XMFLOAT4		Right; 
		XMFLOAT4		Up;
		XMFLOAT4		Look;
		XMFLOAT4		Translation;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance_Declaration
	{
		static const unsigned int				NumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXRECTINSTANCE_DECL;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int				NumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[NumElements];
	}VTXPOINTINSTANCE_DECL;
}
