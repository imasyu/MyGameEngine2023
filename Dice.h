#pragma once
#include "Quad.h"

class Dice : public Quad
{
public:
	//���_���̏���
	void InitVertexData()	override;

	//�C���f�b�N�X��������
	void InitIndexData()	override;
};

