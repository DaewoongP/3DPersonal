#pragma once
#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 6387)

#include <memory>
#include "Windows.h"
#include "Process.h"

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>

#include "fx11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"
#include "DirectXTK\ScreenGrab.h"
#include "DirectXTK\PrimitiveBatch.h"
#include "DirectXTK\VertexTypes.h"
#include "DirectXTK\Effects.h"
#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK\SpriteFont.h"

#include "fmod\fmod.h"
#include "fmod\fmod.hpp"

#include "DirectXTex/DirectXTex.h"

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <array>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <any>
#include <stack>
#include <random>
#include <tchar.h>
#include <ctime>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;
using namespace DirectX;

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Model_Defines.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine::Types;