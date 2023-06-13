#include "Dice.h"

Dice::Dice()
{
}

Dice::~Dice()
{
}

HRESULT Dice::Initialize()
{
	HRESULT hr;

	VERTEX vertices[] =
	{
		//�ʂP
		{XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j

		//��2
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
		{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.5f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
		{XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  0.5f, 0.f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j

		//��3
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f)},   // �E�ʎl�p�`�̒��_�i����j
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f)},  // �E�ʎl�p�`�̒��_�i�����j 
		{XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f)},   // �E�ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f)},  // �E�ʎl�p�`�̒��_�i�E���j

		//��4
		{XMVectorSet(1.0f,  -1.0f, 2.0f, 0.0f),XMVectorSet(0.0f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
		{XMVectorSet(1.0f,  -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  1.0f, 0.5f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j

		//��5
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f)},	// ��ʎl�p�`�̒��_�i����j
		{XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  0.5f, 0.0f, 0.0f)},	// ��ʎl�p�`�̒��_�i�����j	
		{XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  1.0f, 0.0f, 0.0f)},	// ��ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  1.0f, 0.0f, 0.0f)},	// ��ʎl�p�`�̒��_�i�E���j

		//��6
		{XMVectorSet(1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.75f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i����j
		{XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�����j	
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.75f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E��j
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(1.0f,  0.5f, 0.0f, 0.0f)},	// ���ʎl�p�`�̒��_�i�E���j
	};

	//�C���f�b�N�X���
	int index[] = {
	 0,3,1, 0,2,3,
	 4,7,5, 4,6,7,
	 8,11,9, 8,10,11,
	 12,15,13, 12,14,15,
	 16,19,17, 16,18,19,
	 20,23,21, 20,22,23
	};

	CreateBuffers(vertices, 24, index, 36);

	return S_OK;
}

void Dice::Draw(XMMATRIX& worldMatrix)
{
	SetBuffers(36, worldMatrix);
}

void Dice::Release()
{
}