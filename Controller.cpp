#include "Controller.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#define    CAM_TYPE_FIXED 0       //固定
#define    CAM_TYPE_TPS_NO_ROT 1  //三人称（回転無し）
#define    CAM_TYPE_TPS 2         //三人称
#define    CAM_TYPE_FPS 3         //一人称
#define    CAM_TYPE_MAX


Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller"), hModel_(-1), camType_(0)
{
}

void Controller::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets/BoxSand.fbx");
	assert(hModel_ >= 0);

	
}


void Controller::Update()
{
    //左回転
    if (Input::IsKey(DIK_LEFT))
    {
            //何らかの処理
            transform_.rotate_.y -= 1.0f;
    }
    //右回転
    if (Input::IsKey(DIK_RIGHT))
    {
        //何らかの処理
        transform_.rotate_.y += 1.0f;
    }
    //上回転
    if (Input::IsKey(DIK_UP))
    {
        if (transform_.rotate_.x <= 44)
        {
            transform_.rotate_.x += 1.0f;
        }
    }
    //下回転
    if (Input::IsKey(DIK_DOWN))
    {
        if (transform_.rotate_.x >= -44)
        {
            transform_.rotate_.x -= 1.0f;
        }
    }
    

    //回転行列
    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX nRotate = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));

    //現在地をベクトル型に変換
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //前後移動
    XMVECTOR frontMove = XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);  //奥に0.1m
    frontMove = XMVector3TransformCoord(frontMove, mRotate);   //移動ベクトル回転
    
    if (Input::IsKey(DIK_W))
    {
        vPos += frontMove;
    }
    if (Input::IsKey(DIK_S))
    {
        vPos -= frontMove;
    }

    XMVECTOR rightMove = XMVectorSet(0.1f, 0, 0, 0);         //右向きのXMVECTOR構造体を用意
    rightMove = XMVector3TransformCoord(rightMove, mRotate); //移動ベクトル回転
 
    if (Input::IsKey(DIK_D))
    {
        vPos += rightMove;
    }
    if (Input::IsKey(DIK_A))
    {
        vPos -= rightMove;

    }
    XMStoreFloat3(&transform_.position_, vPos);

    //カメラ
    XMVECTOR vCam = XMVectorSet(0, 0, -10, 0);               //自撮り棒を用意
    vCam = XMVector3TransformCoord(vCam, mRotate * nRotate);
    XMFLOAT3 camPos;
    Camera::SetPosition(vPos + vCam);
    Camera::SetTarget(transform_.position_);
}

void Controller::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Controller::Release()
{
}
