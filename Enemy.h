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
};

