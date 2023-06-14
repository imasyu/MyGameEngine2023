#include "Dice.h"

Dice::Dice()
{
}

Dice::~Dice()
{
}

HRESULT Dice::Initialize()
{
	HRESULT hr;

	VERTEX vertices[] =
	{
		//面１
		{XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},	// 正面四角形の頂点（左上）
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},	// 正面四角形の頂点（左下）	
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},	// 正面四角形の頂点（右上）
		{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},	// 正面四角形の頂点（右下）

		//面2
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 左面四角形の頂点（左上）
		{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.5f,  0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 左面四角形の頂点（左下）	
		{XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 左面四角形の頂点（右上）
		{XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  0.5f, 0.f, 0.0f), XMVectorSet(0-1.0f, 0.0f, 0.0f, 0.0f)},	// 左面四角形の頂点（右下）

		//面3
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},   // 右面四角形の頂点（左上）
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},  // 右面四角形の頂点（左下） 
		{XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},   // 右面四角形の頂点（右上）
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},  // 右面四角形の頂点（右下）

		//面4
		{XMVectorSet(1.0f,  -1.0f, 2.0f, 0.0f),XMVectorSet(0.0f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 下面四角形の頂点（左上）
		{XMVectorSet(1.0f,  -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 下面四角形の頂点（左下）	
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 下面四角形の頂点（右上）
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  1.0f, 0.5f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 下面四角形の頂点（右下）

		//面5
		{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  0.5f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 上面四角形の頂点（左上）
		{XMVectorSet(1.0f,  1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  0.5f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 上面四角形の頂点（左下）	
		{XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.25f,  1.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 上面四角形の頂点（右上）
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.5f,  1.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},	// 上面四角形の頂点（右下）

		//面6
		{XMVectorSet(1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.75f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 裏面四角形の頂点（左上）
		{XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 裏面四角形の頂点（左下）	
		{XMVectorSet(-1.0f, 1.0f, 2.0f, 0.0f),XMVectorSet(0.75f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 裏面四角形の頂点（右上）
		{XMVectorSet(-1.0f, -1.0f, 2.0f, 0.0f),XMVectorSet(1.0f,  0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)},	// 裏面四角形の頂点（右下）
	};

	//インデックス情報
	int index[] = {
	 0,3,1, 0,2,3,
	 4,7,5, 4,6,7,
	 8,11,9, 8,10,11,
	 12,15,13, 12,14,15,
	 16,19,17, 16,18,19,
	 20,23,21, 20,22,23
	};

	CreateBuffers(vertices, 24, index, 36);

	return S_OK;
}

void Dice::Draw(XMMATRIX& worldMatrix)
{
	SetBuffers(36, worldMatrix);
}

void Dice::Release()
{
}
