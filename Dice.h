#pragma once
#include "Quad.h"

class Dice : public Quad
{
public:
	//頂点情報の準備
	void InitVertexData()	override;

	//インデックス情報を準備
	void InitIndexData()	override;
};

