#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "resource.h"
#include "Engine/Camera.h"

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++)
    {
        hModel_[i] = -1;
    }
    //table�̏�����
    for (int x = 0; x < MODEL_NUM; x++) {
        for (int z = 0; z < MODEL_NUM; z++) {
            //table_[x][z] = 0;
            SetBlock(x, z, DEFAULT);
        }
    }
}

void Stage::Initialize()
{
    string modelname[] = {
        "BoxDefault.fbx",
        "BoxBrick.fbx",
        "BoxGrass.fbx",
        "BoxSand.fbx",
        "BoxWater.fbx"
    };
    string fname_base = "Assets/";
    //���f���f�[�^�̃��[�h
    for (int i = 0; i < MODEL_NUM; i++)
    {
        hModel_[i] = Model::Load(fname_base + modelname[i]);
        assert(hModel_[i] >= 0);
    }

    //table�Ƀu���b�N�̃^�C�v���Z�b�g
    for (int z = 0; z < ZSIZE; z++) {
        for (int x = 0; x < XSIZE; x++) {
            //table_[x][z] = x % 5;
            SetBlock(x, z, (BLOCKTYPE)(z%5));
            SetBlockHeight(x, z, x%4);
        }
    }
}

void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0)) {
        return;
    }
    float w = (float)(Direct3D::scrWidth / 2);
    float h = (float)(Direct3D::scrHeight / 2);
    //0ffsetx.y��0
    //minZ = 0 maxZ = 1

    XMMATRIX vp =
    {
        w,  0, 0, 0,
        0, -h, 0, 0,
        0,  0, 1, 0,
        w,  0, 0, 1
    };
    //�r���[�|�[�g
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
    //�v���W�F�N�V�����ϊ�
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    //�r���[�ϊ�
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;
    //�@�@mousePosFront���x�N�g���ɕϊ�
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
    //�A�@�@��invVP�AinvPrj�AinvView��������
    vMouseFront = XMVector2TransformCoord(vMouseFront, invVP * invProj * invView);
    //�B�@mousePosBack���x�N�g���ɕϊ�
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
    //�C�@�B��invVP�AinvPrj�AinvView��������
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //�D�@�A����C�Ɍ������ă��C�����i�Ƃ肠�������f���ԍ���hModel_[0]�j
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[0], trans);


                Model::RayCast(hModel_[0], data);

                //�E�@���C������������u���[�N�|�C���g�Ŏ~�߂�
                if (data.hit)
                {
                    break;
                }
            }
        }
    }
    
    
}

void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //table[x][z]����I�u�W�F�N�g�̃^�C�v�����o���ď���
                int type = table_[x][z].type;
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[type], trans);
                Model::Draw(hModel_[type]);
            }
        }
    }
}

void Stage::Release()
{
}

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    //�G���[�`�F�b�N�@�͈͓��̒l���ǂ����@�����ق�����
    table_[_x][_z].type = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    //�G���[�`�F�b�N�@�͈͓��̒l���ǂ����@�����ق�����
    table_[_x][_z].height = _height;
}

//������̃v���V�[�W��
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    //�_�C�A���O�ł���
    case WM_INITDIALOG:
        //���W�I�{�^���̏����l
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);

        //�R���{�{�b�N�X�̏����l
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("�f�t�H���g"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("�����K"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("�K���X"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("��"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("��"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0, 0);
        return TRUE;
    }
    return FALSE;
}