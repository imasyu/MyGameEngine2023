#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"

class Fbx;

class Enemy :
    public GameObject
{
	Fbx* pFbx;
public:
	//�R���X�g���N�^
	Enemy(GameObject* parent);

	//�f�X�g���N�^
	~Enemy();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//���
	void Release() override;

	//�����ɓ�������
	//����pTarget ������������
	void OnCollision(GameObject* pTarget) override;
};

