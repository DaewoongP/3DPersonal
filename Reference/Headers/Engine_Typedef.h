#pragma once

namespace Engine
{
	namespace Types
	{
		typedef		signed char				_byte;
		typedef		char					_char;
		typedef		unsigned char			_ubyte;
		typedef		signed char				_uchar;

		typedef		wchar_t					_tchar;

		typedef		signed short			_short;
		typedef		unsigned short			_ushort;

		typedef		signed int				_int;
		typedef		unsigned int			_uint;

		typedef		signed long				_long;
		typedef		unsigned long			_ulong;

		typedef		float					_float;
		typedef		double					_double;

		typedef		bool					_bool;

		template<typename Key, typename Value>
		using		_umap = std::unordered_map<Key, Value>;

		typedef		DirectX::XMUINT2		_uint2;
		typedef		DirectX::XMUINT3		_uint3;
		typedef		DirectX::XMUINT4		_uint4;

		typedef		DirectX::XMINT2			_int2;
		typedef		DirectX::XMINT3			_int3;
		typedef		DirectX::XMINT4			_int4;

		typedef		DirectX::XMFLOAT2		_float2;
		typedef		DirectX::XMFLOAT3		_float3;
		typedef		DirectX::XMFLOAT4		_float4;

		typedef		DirectX::XMVECTOR		_vector;
		typedef		DirectX::FXMVECTOR		_fvector;

		typedef		DirectX::XMFLOAT4X4		_float4x4;
		typedef		DirectX::XMMATRIX		_matrix;
		typedef		DirectX::FXMMATRIX		_fmatrix;
	}
	
}