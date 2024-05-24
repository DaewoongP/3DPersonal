#include "TestObject.h"

USING(Client)

HRESULT TestObject::Initialize()
{
	AddComponents();
	return S_OK;
}

void TestObject::Tick(_float _timeDelta)
{
}

void TestObject::LateTick(_float _timeDelta)
{
	GAME->AddRenderGroup(Engine::RenderManager::RenderType::PRIORITY, shared_from_this());
}

HRESULT TestObject::Render()
{
	_float4x4 identityMatrix, viewMatrix, projMatrix;

	_float m_fX = gWinSizeX * 0.5f;
	_float m_fY = gWinSizeY * 0.5f;
	XMStoreFloat4x4(&identityMatrix, XMMatrixScaling(gWinSizeX, gWinSizeY, 1.f) * XMMatrixTranslation(m_fX - gWinSizeX * 0.5f, -m_fY + gWinSizeY * 0.5f, 0.f));
	mShader->BindMatrix("gWorldMatrix", &identityMatrix);

	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projMatrix, XMMatrixOrthographicLH(gWinSizeX, gWinSizeY, 0.f, 1.f));

	mShader->BindMatrix("gViewMatrix", &viewMatrix);
	mShader->BindMatrix("gProjMatrix", &projMatrix);

	mTexture->BindShaderResources(mShader, "gTexture");
	/*_float4 color = _float4(1.f, 0.f, 0.f, 1.f);
	mShader->BindRawValue("gColor", &color, sizeof(_float4));*/

	mShader->Begin("Test");

	mRectBuffer->Render();

	return S_OK;
}

HRESULT TestObject::AddComponents()
{
	mRectBuffer = Engine::RectBuffer::Create();
	mShader = Engine::Shader::Create(TEXT("../Bin/ShaderFiles/VtxtexShader.hlsl"), Engine::VTXPOSTEX_DECL::Elements, Engine::VTXPOSTEX_DECL::NumElements);
	mTexture = Engine::Texture::Create(TEXT("../../Resource/Default/Textures/Default0.dds"));
	return S_OK;
}

std::shared_ptr<TestObject> TestObject::Create()
{
	auto instance = std::make_shared<TestObject>();
	instance->Initialize();
	return instance;
}

std::shared_ptr<Engine::Component> Client::TestObject::Clone(void* _arg)
{
	return std::shared_ptr<Engine::Component>();
}
