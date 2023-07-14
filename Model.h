#pragma once
#include <string>
#include <vector>
#include "Engine/Fbx.h"

namespace Model
{
	
	int Load(std::string filenNme);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release();

}

