#pragma once
#include "Engine/GameObject.h"
#include "windows.h"

namespace {
	const int MODEL_NUM{ 5 };
	const int XSIZE{ 15 };
	const int ZSIZE{ 15 };

	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}

//�V�[�����Ǘ�����N���X
class Stage : public GameObject
{
	struct BLOCK {
		int type;
		int height;
	}table_[XSIZE][ZSIZE];
	//table_[15][15];

	int hModel_[MODEL_NUM]; //���f���ԍ�

	int mode_;   //0:�グ��@1:������@2:��ނ�ς���
	int select_; //���
	int controlId; //�R���g���[����ID���擾
	int comboId; //�R���{��ID���擾
	int menuId;
	bool rayHit_;   //���C���q�b�g�������ǂ����������t���O
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Stage(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetBlock(int _x, int _z, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

};
