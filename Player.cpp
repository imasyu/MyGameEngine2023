#include "Player.h"
#include "Engine/Fbx.h"
#include "Engine/Input.h"
#include "Childoden.h"

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
    transform_.scale_.x = 0.8;
    transform_.scale_.y = 0.8;
    transform_.scale_.z = 0.8;
    //Instantiate<Childoden>(this);
    //GameObject *pCO2 = Instantiate<Childoden>(this);
    //pCO2->SetPosition(-2, 1, 0);
}

void Player::Update()
{
    transform_.rotate_.y++;

    //if (transform_.rotate_.y > 600)
    //{
    //    KillMe();
    //}
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.position_.x -= 0.1f;
    }
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.position_.x += 0.1f;
    }
    if (Input::IsKeyUp(DIK_A))
    {
        KillMe();
    }
    if (Input::IsKeyDown(DIK_SPACE))
    {
        GameObject* pBullet = Instantiate<Childoden>(pParent_);
        pBullet->SetPosition(transform_.position_);
    }
}

void Player::Draw()
{
    pFbx->Draw(transform_);
}

void Player::Release()
{
    //pFbx->Release();
    //delete pFbx;
}
