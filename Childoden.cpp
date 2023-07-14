#include "Childoden.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"

Childoden::Childoden(GameObject* parent)
    :GameObject(parent, "Childoden"), pFbx(nullptr)
{
}

Childoden::~Childoden()
{
}

void Childoden::Initialize()
{
    pFbx = new Fbx;
    pFbx->Load("Assets/Oden.fbx");
    transform_.scale_.x = 0.2;
    transform_.scale_.y = 0.2;
    transform_.scale_.z = 0.2;
    transform_.position_.x = 2.0f;
    transform_.position_.y = 1.0f;
}

void Childoden::Update()
{
    transform_.rotate_.y ++;
    transform_.rotate_.x += 10.0f;
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
    pFbx->Draw(transform_);
}

void Childoden::Release()
{
    pFbx->Release();
    delete pFbx;
}
