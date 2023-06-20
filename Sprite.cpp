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

//�������i�|���S����\�����邽�߂̊e����������j
//�߂�l�F����/���s
HRESULT Sprite::Initialize()
{
    //���_���
    InitVertexData();

    //���_�o�b�t�@�쐬
    HRESULT hr = CreateVertexBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //�C���f�b�N�X���
    InitIndexData();

    //�C���f�b�N�X�o�b�t�@�쐬
    hr = CreateIndexBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //�R���X�^���g�o�b�t�@�쐬
    hr = CreateConstantBuffer();
    if (FAILED(hr))
    {
        return hr;
    }

    //�e�N�X�`���̃��[�h
    hr = LoadTexture();
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

//�`��
//�����FworldMatrix�@���[���h�s��
void Sprite::Draw(XMMATRIX& worldMatrix)
{
    PassDataToCB(worldMatrix);
    SetBufferToPipeline();

    Direct3D::pContext_->DrawIndexed(indexNum, 0, 0);
}

//���
void Sprite::Release()
{
    SAFE_DELETE(pTexture_);
    SAFE_RELEASE(pConstantBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pVertexBuffer_);
}

//���_���̏���
void Sprite::InitVertexData()
{
    // ���_���
    vertices_ =
    {
        {XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
    };

    //���_�̐�
    vertexNum_ = vertices_.size();	//�S�f�[�^�̃T�C�Y�@���@1���_���̃T�C�Y�@���@���_��

}

//���_�o�b�t�@���쐬
HRESULT Sprite::CreateVertexBuffer()
{
    HRESULT hr;
    // ���_�f�[�^�p�o�b�t�@�̐ݒ�
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
        MessageBox(nullptr, "���_�f�[�^�p�o�b�t�@�̐ݒ�ɃG���[���N���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
    }
    return S_OK;
}

//�C���f�b�N�X��������
void Sprite::InitIndexData()
{
    //�C���f�b�N�X���
    int index[] = {0,2,3, 0,1,2};

    //�C���f�b�N�X��
    indexNum = index_.size();	
}

//�C���f�b�N�X�o�b�t�@���쐬
HRESULT Sprite::CreateIndexBuffer()
{
    HRESULT hr;
    // �C���f�b�N�X�o�b�t�@�𐶐�����
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
        MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̐����Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
    }
    return S_OK;
}

//�R���X�^���g�o�b�t�@�쐬
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
        MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
        //�G���[����
    }
    return S_OK;
}

//�e�N�X�`�������[�h
HRESULT Sprite::LoadTexture()
{
    HRESULT hr;
    pTexture_ = new Texture;
    hr = pTexture_->Load("Assets\\dice.png");
    if (FAILED(hr))
    {
        MessageBox(NULL, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�R���X�^���g�o�b�t�@�Ɋe�����n��
void Sprite::PassDataToCB(DirectX::XMMATRIX& worldMatrix)
{
    CONSTANT_BUFFER cb;
    cb.matW = XMMatrixTranspose(worldMatrix);

    D3D11_MAPPED_SUBRESOURCE pdata;
    Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

    ID3D11SamplerState* pSampler = pTexture_->GetSampler();
    Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

    ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
    Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

    Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
}

//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
void Sprite::SetBufferToPipeline()
{
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
}
