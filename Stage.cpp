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
            //SetBlock(x, z, (BLOCKTYPE)(z%5));
            //SetBlockHeight(x, z, x%4);
            SetBlock(x, z, (BLOCKTYPE)(0));
            SetBlockHeight(x, z, 0);
        }
    }
    controlId = IDC_RADIO_UP;
}

void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0)) {
        return;
    }
    float w = (float)(Direct3D::scrWidth / 2.0f);
    float h = (float)(Direct3D::scrHeight / 2.0f);
    //0ffsetx.y��0
    //minZ = 0 maxZ = 1

    XMMATRIX vp =
    {
        w,  0, 0, 0,
        0, -h, 0, 0,
        0,  0, 1, 0,
        w,  h, 0, 1
    };

    //�r���[�|�[�g
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
    //�v���W�F�N�V�����ϊ�
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    //�r���[�ϊ�
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
    //���C��-5���ړ������āA���߂���
    float angleIncrement = XMConvertToRadians(-5.0f); // �p�x�����W�A���ɕϊ�
    XMMATRIX rotationMatrix = XMMatrixRotationX(angleIncrement); // X������ɉ�]
    invView = XMMatrixMultiply(rotationMatrix, invView); // �r���[�s��ɉ�]�s���K�p
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

    rayHit_ = false;

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
                if (data.hit && !rayHit_) {
                    rayHit_ = true;
                    if (controlId == IDC_RADIO_UP) {
                        table_[x][z].height++;
                        break;
                    }
                    else if (controlId == IDC_RADIO_DOWN) {
                        if (y > 0) {
                            table_[x][z].height--;
                            break;
                        }
                    }
                    else if(controlId == IDC_COMBO1) {
                        SetBlock(x, z, (BLOCKTYPE)(comboId));
                        break;
                    }
                        
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

void Stage::Save()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    HANDLE hFile;
    hFile = CreateFile(
        fileName,    //�t�@�C����
        GENERIC_WRITE,  //�A�N�Z�X���[�h
        0,
        NULL,
        CREATE_ALWAYS,     //�쐬���@
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    std::string data = "";

    //data.length()
    DWORD bytes = 0;
    WriteFile(
        hFile,              //�t�@�C���n���h��
        "ABCDEF\0",          //�ۑ�������������
        12,                  //�ۑ����镶����
        &bytes,             //�ۑ������T�C�Y
        NULL
    );

    CloseHandle(hFile);
}

void Stage::Load()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_FILEMUSTEXIST;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

    //�u�t�@�C�����J���v�_�C�A���O
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        "filename",                 //�t�@�C����
        GENERIC_READ,           //�A�N�Z�X���[�h�i�ǂݍ��ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j

    //�t�@�C���̃T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);

    //�t�@�C���̃T�C�Y�����������m��
    char* data;
    data = new char[fileSize];

    DWORD bytes = 0; //�ǂݍ��݈ʒu
    ReadFile(
        hFile,     //�t�@�C���n���h��
        data,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &bytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j


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
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("��"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("��"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("��"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0, 0);
        return 0;
    case WM_COMMAND:
        controlId = LOWORD(wp);
        comboId = SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
        return TRUE;
    }
    return FALSE;
}
