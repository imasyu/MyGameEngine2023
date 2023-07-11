#include "Childoden.h"
#include "Engine/Fbx.h"

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
    transform_.scale_.x = 0.5;
    transform_.scale_.y = 0.5;
    transform_.scale_.z = 0.5;
    transform_.position_.x = -3;
}

void Childoden::Update()
{
    transform_.rotate_.y ++;
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
