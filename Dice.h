#pragma once
#include "Quad.h"

class Dice : public Quad
{
public:
	Dice();
	~Dice();
	HRESULT Initialize()override;
	void Draw(XMMATRIX& worldMatrix)override;
	void Release();
};

