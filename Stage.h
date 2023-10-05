#pragma once
#include "Engine/GameObject.h"
#include "windows.h"

namespace {
	const int MODEL_NUM{ 5 };
	const int XSIZE{ 15 };
	const int ZSIZE{ 15 };

	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}

//シーンを管理するクラス
class Stage : public GameObject
{
	struct BLOCK {
		int type;
		int height;
	}table_[XSIZE][ZSIZE];
	//table_[15][15];

	int hModel_[MODEL_NUM]; //モデル番号

	int mode_;   //0:上げる　1:下げる　2:種類を変える
	int select_; //種類
	int controlId; //コントロールのIDを取得
	int comboId; //コンボのIDを取得
	int menuId;
	bool rayHit_;   //レイがヒットしたかどうかを示すフラグ
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Stage(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetBlock(int _x, int _z, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

};
