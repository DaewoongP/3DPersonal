#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class TestObject : public Engine::Composite
{
public:
	TestObject() = default;
	~TestObject() = default;

public:
	HRESULT Initialize();
	void Tick(_float _timeDelta);
	void LateTick(_float _timeDelta);
	HRESULT Render();

private:
	HRESULT AddComponents();

private:
	std::shared_ptr<Engine::RectBuffer> mRectBuffer;
	std::shared_ptr<Engine::Shader>		mShader;
	std::shared_ptr<Engine::Texture>	mTexture;

public:
	static std::shared_ptr<TestObject> Create();
	virtual std::shared_ptr<Engine::Component> Clone(void* _arg);
};

END