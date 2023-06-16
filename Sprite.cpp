#include "Sprite.h"

Sprite::Sprite() //:pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr)
{
}

Sprite::~Sprite()
{
	//Release();
}

HRESULT Sprite::Initialize()
{
	return E_NOTIMPL;
}

void Sprite::Draw(XMMATRIX& worldMatrix)
{
}

void Sprite::Release()
{
}

void Sprite::InitVertexData()
{
}

void Sprite::InitIndexData()
{
}

HRESULT Sprite::CreateIndexBuffer()
{
	return E_NOTIMPL;
}

HRESULT Sprite::CreateConstantBuffer()
{
	return E_NOTIMPL;
}

HRESULT Sprite::LoadTexture()
{
	return E_NOTIMPL;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX& worldMatrix)
{
}

void Sprite::SetBufferToPipeline()
{
}
