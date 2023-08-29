#include "Block.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#define    CAM_TYPE_FIXED 0       //�Œ�
#define    CAM_TYPE_TPS_NO_ROT 1  //�O�l�́i��]�����j
#define    CAM_TYPE_TPS 2         //�O�l��
#define    CAM_TYPE_FPS 3         //��l��
#define    CAM_TYPE_MAX


Block::Block(GameObject* parent)
	:GameObject(parent, "Block"), hModel_(-1), camType_(0)
{
}

void Block::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets/BoxSand.fbx");
	assert(hModel_ >= 0);

	
}


void Block::Update()
{
    // A��������Ă�����
        if (Input::IsKey(DIK_A))
        {
            //���炩�̏���
            transform_.rotate_.y -= 2;
        }

    //D��������Ă�����
    if (Input::IsKey(DIK_D))
    {
        //���炩�̏���
        transform_.rotate_.y += 2;
    }

    //���ݒn���x�N�g���^�ɕϊ�
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //1�t���[���̈ړ��x�N�g��
    XMVECTOR vMove{ 0.0f, 0.0f, 0.1f, 0.0f };  //����0.1m

    //transform_.rotate_.y�x��]������s��
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //�ړ��x�N�g����ό`�i���������ɉ�]�j������
    vMove = XMVector3TransformCoord(vMove, mRotY);

    //W��������Ă�����
    if (Input::IsKey(DIK_W))
    {
        //�ړ�
        vPos += vMove;

        //���ݒn���x�N�g�����炢����transform_.position_�ɖ߂�
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
