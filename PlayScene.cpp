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
   //pPlayer = new Player(this); //プレイヤーの親は、自分（PlayScene）
   //pPlayer->Initialize();
   //childList_.pushback(pPlayer); //PlaySceneの子として、プレイヤーを登録
   Instantiate<Player>(this);
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
