#include "Player.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"

Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), pFbx(nullptr)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
    pFbx = new Fbx;
    pFbx->Load("Assets/Oden.fbx");
    transform_.scale_.x = 0.5;
    transform_.scale_.y = 0.5;
    transform_.scale_.z = 0.5;
}

void Player::Update()
{
    transform_.rotate_.y++;

    if (Input::IsKeyUp(DIK_A))
    {
        KillMe();
    }
}

void Player::Draw()
{
    pFbx->Draw(transform_);
}

void Player::Release()
{
    pFbx->Release();
    delete pFbx;
}
