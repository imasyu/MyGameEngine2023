#pragma once
#include "Engine/GameObject.h"

class Fbx;

class Player : public GameObject
{
	Fbx* pFbx;
public:
	//コンストラクタ
	Player(GameObject* parent);

	//デストラクタ
	~Player();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Release() override;
};

