#pragma once
#include "Engine/GameObject.h"

//�O���錾
class Fbx;

class PlayScene :
    public GameObject
{
    Fbx* pFbx;
public:
    //�R���X�g���N�^
 //�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
    PlayScene(GameObject* parent);

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};
