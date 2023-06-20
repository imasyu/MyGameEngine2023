#include "Sprite.h"
#include "Camera.h"

Sprite::Sprite() : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr),
vertexNum_(0), indexNum(0)
{
}

Sprite::~Sprite()
{
	Release();
}

//初期化（ポリゴンを表示するための各種情報を準備）
//戻り値：成功/失敗
HRESULT Sprite::Initialize()
{
    //頂点情報
    InitVertexData();

    //頂点バッファ作成
    HRESULT hr = CreateVertexBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //インデックス情報
    InitIndexData();

    //インデックスバッファ作成
    hr = CreateIndexBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //コンスタントバッファ作成
    hr = CreateConstantBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //テクスチャのロード
    hr = LoadTexture();
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

//描画
//引数：worldMatrix　ワールド行列
void Sprite::Draw(XMMATRIX& worldMatrix)
{
    PassDataToCB(worldMatrix);
    SetBufferToPipeline();

    Direct3D::pContext_->DrawIndexed(indexNum, 0, 0);
}

//解放
void Sprite::Release()
{
    SAFE_DELETE(pTexture_);
    SAFE_RELEASE(pConstantBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pVertexBuffer_);
}

//頂点情報の準備
void Sprite::InitVertexData()
{
    // 頂点情報
    vertices_ =
    {
        {XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左上）
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右上）
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（右下）
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// 正面四角形の頂点（左下）	
    };

    //頂点の数
    vertexNum_ = vertices_.size();	//全データのサイズ　÷　1頂点分のサイズ　＝　頂点数

}

//頂点バッファを作成
HRESULT Sprite::CreateVertexBuffer()
{
    HRESULT hr;
    // 頂点データ用バッファの設定
    D3D11_BUFFER_DESC bd_vertex;
    bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
    bd_vertex.Usage = D3D11_USAGE_DEFAULT;
    bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd_vertex.CPUAccessFlags = 0;
    bd_vertex.MiscFlags = 0;
    bd_vertex.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA data_vertex;
    data_vertex.pSysMem = vertices_.data();
    hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "頂点データ用バッファの設定にエラーが起きました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
    return S_OK;
}

//インデックス情報を準備
void Sprite::InitIndexData()
{
    //インデックス情報
    int index[] = {0,2,3, 0,1,2};

    //インデックス数
    indexNum = index_.size();	
}

//インデックスバッファを作成
HRESULT Sprite::CreateIndexBuffer()
{
    HRESULT hr;
    // インデックスバッファを生成する
    D3D11_BUFFER_DESC   bd;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(int) * indexNum;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = index_.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "インデックスバッファの生成に失敗しました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
    return S_OK;
}

//コンスタントバッファ作成
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
        MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
        return hr;
        //エラー処理
    }
    return S_OK;
}

//テクスチャをロード
HRESULT Sprite::LoadTexture()
{
    HRESULT hr;
    pTexture_ = new Texture;
    hr = pTexture_->Load("Assets\\dice.png");
    if (FAILED(hr))
    {
        MessageBox(NULL, "テクスチャの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }
    return S_OK;
}

//コンスタントバッファに各種情報を渡す
void Sprite::PassDataToCB(DirectX::XMMATRIX& worldMatrix)
{
    CONSTANT_BUFFER cb;
    cb.matW = XMMatrixTranspose(worldMatrix);

    D3D11_MAPPED_SUBRESOURCE pdata;
    Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

    ID3D11SamplerState* pSampler = pTexture_->GetSampler();
    Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

    ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
    Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

    Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開
}

//各バッファをパイプラインにセット
void Sprite::SetBufferToPipeline()
{
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
}
