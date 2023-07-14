#include "Childoden.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Model.h"

Childoden::Childoden(GameObject* parent)
    :GameObject(parent, "Childoden"), pFbx(nullptr), hModel_(-1)
{
}

Childoden::~Childoden()
{
}

void Childoden::Initialize()
{
    hModel_ = Model::Load("Assets/Oden.fbx");
    assert(hModel_ >= 0);
    transform_.scale_.x = 0.2;
    transform_.scale_.y = 0.2;
    transform_.scale_.z = 0.2;
    transform_.position_.x = 2.0f;
    transform_.position_.y = 1.0f;
}

void Childoden::Update()
{
    //transform_.rotate_.y ++;
    transform_.rotate_.y += 0.5f;
    transform_.rotate_.z += 0.5f;
    transform_.position_.z += 0.5f;
    //if (transform_.rotate_.y++ > 300)
    //    KillMe();
    if (transform_.position_.z > 50)
    {
        KillMe();
    }
}

void Childoden::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Childoden::Release()
{
    pFbx->Release();
    delete pFbx;
}
