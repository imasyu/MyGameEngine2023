#include "Sprite.h"
#include "Camera.h"

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
    HRESULT hr;
    hr = LoadTexture(); // テクスチャの読み込み
    if (FAILED(hr)) {
        // エラーハンドリング
        return hr;
    }

    hr = CreateVertexBuffer(); // 頂点バッファの作成
    if (FAILED(hr)) {
        // エラーハンドリング
        return hr;
    }

    hr = CreateIndexBuffer(); // インデックスバッファの作成
    if (FAILED(hr)) {
        // エラーハンドリング
        return hr;
    }

    hr = CreateConstantBuffer(); // コンスタントバッファの作成
    if (FAILED(hr)) {
        // エラーハンドリング
        return hr;
    }
    return S_OK;
}

void Sprite::Draw(XMMATRIX& worldMatrix)
{
    PassDataToCB(worldMatrix);
}

void Sprite::Release()
{
    pTexture_->Release();
    SAFE_DELETE(pTexture_);
    SAFE_RELEASE(pConstantBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData(VERTEX* _ver, int vn, int* _index, int in)
{
    HRESULT hr;
    // 頂点情報
    VERTEX vertices[] =
    {
        {XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左上）
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右上）
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右下）
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左下）	
    };
}

HRESULT Sprite::CreateVertexBuffer()
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
    //インデックス情報
    int index[] = {0,2,3, 0,1,2};

    CreateIndexBuffer(vertices_, 4, index, 6);
}


HRESULT Sprite::CreateIndexBuffer(VERTEX* _ver, int vn, int* _index, int in)
{
    HRESULT hr;
    // インデックスバッファを生成する
    D3D11_BUFFER_DESC   bd;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(int) * _in;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = _index;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "インデックスバッファの生成に失敗しました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
}

HRESULT Sprite::CreateConstantBuffer()
{
    HRESULT hr;
    //コンスタントバッファ作成
    D3D11_BUFFER_DESC cb;
    cb.ByteWidth = sizeof(CONSTANT_BUFFER);
    cb.Usage = D3D11_USAGE_DYNAMIC;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;

    // コンスタントバッファの作成
    hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
    return S_OK;
}

HRESULT Sprite::LoadTexture()
{
    pTexture_ = new Texture;
    pTexture_->Load("Assets\\dice.png");
    return S_OK;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX& worldMatrix)
{
    CONSTANT_BUFFER cb;
    cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
    cb.matW = XMMatrixTranspose(worldMatrix);

    D3D11_MAPPED_SUBRESOURCE pdata;
    Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

    ID3D11SamplerState* pSampler = LoadTexture_->GetSampler();
    Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

    ID3D11ShaderResourceView* pSRV = LoadTexture_->GetSRV();
    Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
}

void Sprite::SetBufferToPipeline()
{
    Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開
    //頂点バッファ
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
    // インデックスバッファーをセット
    stride = sizeof(int);
    offset = 0;
    Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

    //コンスタントバッファ
    Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
    Direct3D::pContext_->PSSetConstantBuffers(0, 0, &pConstantBuffer_);	//ピクセルシェーダー用

    Direct3D::pContext_->DrawIndexed(_in, 0, 0);
}
