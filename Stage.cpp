#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

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
