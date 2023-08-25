#include "Stage.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_{-1, -1 }, width_(0), height_(0)
{

        table_ = new int* [width_];

        for (int x = 0; x < width_; x++)
        {
            table_[x] = new int[height_];
        }
}

void Stage::Initialize()
{
    const char* fileName[] = {
        "Assets/BoxDefault.fbx"
    };

    //モデルデータのロード
    for (int i = 0; i < TYPE_MAX; i++)
    {
        hModel_[i] = Model::Load(fileName[i]);
        assert(hModel_[i] >= 0);
    }
}

void Stage::Update()
{
}

void Stage::Draw()
{
    Transform blockTrans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);

        }
    }
}

void Stage::Release()
{
    for (int x = 0; x < height_; x++)
    {
        delete[] table_[x];
    }
    delete[] table_;
}
