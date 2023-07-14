#pragma once
#include <string>
#include <vector>
#include "Engine/Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pfbx_;
		Transform transform_;
		std::string filename_;
		//トランスフォーム
	};
	int Load(std::string filenNme);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	//モデルのポインタをぶち込んでおくベクタ
	std::vector<ModelData*> modelList;
}

