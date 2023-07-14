#include "PlayScene.h"
#include "Engine/Fbx.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
    :GameObject(parent, "PlayScene"),
    pFbx(nullptr)
{

}

void PlayScene::Initialize()
{
   //Player* pPlayer;
   //pPlayer = new Player(this); //�v���C���[�̐e�́A�����iPlayScene�j
   //pPlayer->Initialize();
   //childList_.pushback(pPlayer); //PlayScene�̎q�Ƃ��āA�v���C���[��o�^
   GameObject* pPlayer = Instantiate<Player>(this);
}

void PlayScene::Update()
{
    
}

void PlayScene::Draw()
{
    
}

void PlayScene::Release()
{
    pFbx->Release();
    delete pFbx;
}