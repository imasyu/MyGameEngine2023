#include "Controller.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#define    CAM_TYPE_FIXED 0       //�Œ�
#define    CAM_TYPE_TPS_NO_ROT 1  //�O�l�́i��]�����j
#define    CAM_TYPE_TPS 2         //�O�l��
#define    CAM_TYPE_FPS 3         //��l��
#define    CAM_TYPE_MAX


Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller"), hModel_(-1), camType_(0)
{
}

void Controller::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets/BoxSand.fbx");
	assert(hModel_ >= 0);

	
}


void Controller::Update()
{
    //����]
    if (Input::IsKey(DIK_LEFT))
    {
            //���炩�̏���
            transform_.rotate_.y -= 1.0f;
    }
    //�E��]
    if (Input::IsKey(DIK_RIGHT))
    {
        //���炩�̏���
        transform_.rotate_.y += 1.0f;
    }
    //���]
    if (Input::IsKey(DIK_UP))
    {
        if (transform_.rotate_.x <= 44)
        {
            transform_.rotate_.x += 1.0f;
        }
    }
    //����]
    if (Input::IsKey(DIK_DOWN))
    {
        if (transform_.rotate_.x >= -44)
        {
            transform_.rotate_.x -= 1.0f;
        }
    }
    

    //��]�s��
    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX nRotate = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));

    //���ݒn���x�N�g���^�ɕϊ�
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //�O��ړ�
    XMVECTOR frontMove = XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);  //����0.1m
    frontMove = XMVector3TransformCoord(frontMove, mRotate);   //�ړ��x�N�g����]
    
    if (Input::IsKey(DIK_W))
    {
        vPos += frontMove;
    }
    if (Input::IsKey(DIK_S))
    {
        vPos -= frontMove;
    }

    XMVECTOR rightMove = XMVectorSet(0.1f, 0, 0, 0);         //�E������XMVECTOR�\���̂�p��
    rightMove = XMVector3TransformCoord(rightMove, mRotate); //�ړ��x�N�g����]
 
    if (Input::IsKey(DIK_D))
    {
        vPos += rightMove;
    }
    if (Input::IsKey(DIK_A))
    {
        vPos -= rightMove;

    }
    XMStoreFloat3(&transform_.position_, vPos);

    //�J����
    XMVECTOR vCam = XMVectorSet(0, 0, -10, 0);               //���B��_��p��
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
