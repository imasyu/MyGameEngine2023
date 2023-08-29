#include "Block.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#define    CAM_TYPE_FIXED 0       //固定
#define    CAM_TYPE_TPS_NO_ROT 1  //三人称（回転無し）
#define    CAM_TYPE_TPS 2         //三人称
#define    CAM_TYPE_FPS 3         //一人称
#define    CAM_TYPE_MAX


Block::Block(GameObject* parent)
	:GameObject(parent, "Block"), hModel_(-1), camType_(0)
{
}

void Block::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets/BoxSand.fbx");
	assert(hModel_ >= 0);

	
}


void Block::Update()
{
    // Aが押されていたら
        if (Input::IsKey(DIK_A))
        {
            //何らかの処理
            transform_.rotate_.y -= 2;
        }

    //Dが押されていたら
    if (Input::IsKey(DIK_D))
    {
        //何らかの処理
        transform_.rotate_.y += 2;
    }

    //現在地をベクトル型に変換
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //1フレームの移動ベクトル
    XMVECTOR vMove{ 0.0f, 0.0f, 0.1f, 0.0f };  //奥に0.1m

    //transform_.rotate_.y度回転させる行列
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //移動ベクトルを変形（同じ向きに回転）させる
    vMove = XMVector3TransformCoord(vMove, mRotY);

    //Wが押されていたら
    if (Input::IsKey(DIK_W))
    {
        //移動
        vPos += vMove;

        //現在地をベクトルからいつものtransform_.position_に戻す
        //XMStoreFloat3(&transform_.position_, vPos);

    }

    if (Input::IsKeyDown(DIK_Z))
    {
        camType_++;
        if (camType_ == 4)
        {
            camType_ = 0;
        }
    }

    switch (camType_)
    {
    case 0:
    {
        Camera::SetTarget(XMFLOAT3(0, 0, 0));
        Camera::SetPosition(XMFLOAT3(0, 20, -30));
        break;
    }
    case 1:
    {
        Camera::SetTarget(transform_.position_);
        XMFLOAT3 camPos = transform_.position_;
        camPos.y += 5;
        camPos.z -= 10;
        Camera::SetPosition(camPos);
        break;
    }
    case 2:
    {
        Camera::SetTarget(transform_.position_);
        XMVECTOR vCam = { 0, 5, -10, 0 };
        vCam = XMVector3TransformCoord(vCam, mRotY);
        XMFLOAT3 camPos;
        XMStoreFloat3(&camPos, vPos + vCam);
        Camera::SetPosition(camPos);
        break;
    }
    case 3:
    {
        Camera::SetTarget(transform_.position_);
        XMFLOAT3 camTarget;
        XMStoreFloat3(&camTarget, vPos + vMove);
        Camera::SetTarget(camTarget);
        break;
    }
    }

    Camera::SetPosition(XMFLOAT3(transform_.position_));

    XMVECTOR vCam = { 0, 5, -10, 0 };
    vCam = XMVector3TransformCoord(vCam, mRotY);
    XMFLOAT3 camPos;
    XMStoreFloat3(&camPos, vPos + vCam);
    Camera::SetPosition(camPos);
}

void Block::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Block::Release()
{
}
