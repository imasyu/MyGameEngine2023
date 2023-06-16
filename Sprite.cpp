#include "Sprite.h"

Sprite::Sprite() : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr),
vertexNum_(0), indexNum_(0)
{
}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Initialize()
{
    HRESULT hr = InitVertexData();
    if (FAILED(hr))
        return hr;

    hr = InitIndexData();
    if (FAILED(hr))
        return hr;

    hr = CreateConstantBuffer();
    if (FAILED(hr))
        return hr;

    hr = LoadTexture();
    if (FAILED(hr))
        return hr;

    return S_OK;  // 初期化が成功した場合は S_OK を返す
}

void Sprite::Draw(XMMATRIX& worldMatrix)
{
}

void Sprite::Release()
{
}

void Sprite::InitVertexData(VERTEX* _ver, int vn, int* _index, int in)
{
    HRESULT hr;
    // 頂点情報
    VERTEX vertices[] =
    {
        {XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左上）
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右上）
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右下）
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左下）	
    };

    //インデックス情報
    int index[] = { 0,2,3, 0,1,2 };

    CreateBuffers(vertices, 4, index, 6);

    return S_OK;
}

HRESULT Sprite::CreateIndexBuffer()
{
    HRESULT hr;
    // 頂点データ用バッファの設定
    D3D11_BUFFER_DESC bd_vertex;
    bd_vertex.ByteWidth = sizeof(VERTEX) * _vn;
    bd_vertex.Usage = D3D11_USAGE_DEFAULT;
    bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd_vertex.CPUAccessFlags = 0;
    bd_vertex.MiscFlags = 0;
    bd_vertex.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA data_vertex;
    data_vertex.pSysMem = vertices_;
    hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "頂点データ用バッファの設定にエラーが起きました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
}

void Sprite::InitIndexData()
{

}

HRESULT Sprite::CreateIndexBuffer(VERTEX* _ver, int vn, int* _index, int in)
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
