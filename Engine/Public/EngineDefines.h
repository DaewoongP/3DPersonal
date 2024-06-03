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

#ifdef _DEBUG
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "ImGuiFileDialog.h"
#endif // _DEBUG

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

#include "EngineTypedef.h"
#include "EngineMacro.h"
#include "EngineStruct.h"
#include "EngineFunction.h"
#include "EngineModelDefines.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define _new DBG_NEW

#endif
#else // Release
#define _new	new

#endif // _DEBUG

using namespace Engine::Types;