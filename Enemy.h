#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"

class Fbx;

class Enemy :
    public GameObject
{
	Fbx* pFbx;
public:
	//コンストラクタ
	Enemy(GameObject* parent);

	//デストラクタ
	~Enemy();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Release() override;

	//何かに当たった
	//引数pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;
};

