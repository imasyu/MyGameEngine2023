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
    // ���_���
    VERTEX vertices[] =
    {
        {XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
    };
}

HRESULT Sprite::CreateVertexBuffer()
{
    HRESULT hr;
    // ���_�f�[�^�p�o�b�t�@�̐ݒ�
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
        MessageBox(nullptr, "���_�f�[�^�p�o�b�t�@�̐ݒ�ɃG���[���N���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
    }
}

void Sprite::InitIndexData()
{
    //�C���f�b�N�X���
    int index[] = {0,2,3, 0,1,2};

    CreateIndexBuffers(vertices_, 4, index_, 6);

    return S_OK;
}


HRESULT Sprite::CreateIndexBuffer(VERTEX* _ver, int vn, int* _index, int in)
{
    HRESULT hr;
    // �C���f�b�N�X�o�b�t�@�𐶐�����
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
        MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̐����Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
    }
}

HRESULT Sprite::CreateConstantBuffer()
{
    HRESULT hr;
    //�R���X�^���g�o�b�t�@�쐬
    D3D11_BUFFER_DESC cb;
    cb.ByteWidth = sizeof(CONSTANT_BUFFER);
    cb.Usage = D3D11_USAGE_DYNAMIC;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;

    // �R���X�^���g�o�b�t�@�̍쐬
    hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
    if (FAILED(hr))
    {
        MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
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
    Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

    ID3D11SamplerState* pSampler = LoadTexture_->GetSampler();
    Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

    ID3D11ShaderResourceView* pSRV = LoadTexture_->GetSRV();
    Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
}

void Sprite::SetBufferToPipeline()
{
    Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
    //���_�o�b�t�@
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
    // �C���f�b�N�X�o�b�t�@�[���Z�b�g
    stride = sizeof(int);
    offset = 0;
    Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

    //�R���X�^���g�o�b�t�@
    Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
    Direct3D::pContext_->PSSetConstantBuffers(0, 0, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

    Direct3D::pContext_->DrawIndexed(_in, 0, 0);
}