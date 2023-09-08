#include "Model.h"
#include "Direct3D.h"

struct ModelData
{
	Fbx* pfbx_;
	Transform transform_;
	std::string filename_;
	//トランスフォーム
};
//モデルのポインタをぶち込んでおくベクタ
std::vector<ModelData*> modelList;

int Model::Load(std::string fileName)
{
	//よんでつくる
	//modelListにぶっこむ
	//今呼んだモデルのモデル番号を返す！
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = fileName;
	pData->pfbx_ = nullptr;

	//ファイルネームが同じだったら、読まない
	for (auto e : modelList)
	{
		if (e->filename_ == fileName) {
			pData->pfbx_ = e->pfbx_;
			break;
		}
	}

	if (pData->pfbx_ == nullptr)
	{
		pData->pfbx_ = new Fbx;
		pData->pfbx_->Load(fileName);
	}
	modelList.push_back(pData);
	return(modelList.size() - 1);
}

void Model::SetTransform(int hModel, Transform transform)
{
	//モデル番号は、modelListのインデックス
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	//モデル番号は、modelListのインデックス
	modelList[hModel]->pfbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false; //参照されている？
	for (int i = 0; i < modelList.size(); i++)
	{
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->pfbx_ == modelList[i]->pfbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pfbx_);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear();
}

void Model::RayCast(int hModel, RayCastData& rayData)
{
	//⓪モデルのトランスフォームをカリキュレーション
	modelList[hModel]->transform_.Calclation();
	//①ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr, modelList[hModel]->transform_.GetWorldMatrix());
	//②レイの通過点を求める(モデル空間でのレイの方向ベクトルを求める)
	XMVECTOR vpass{ rayData.start.x + rayData.dir.x,
					rayData.start.y + rayData.dir.y,
					rayData.start.z + rayData.dir.z,
					rayData.start.w + rayData.dir.w };
	//③rayData.startをモデル空間にへんかん（①をかける）
	XMVECTOR vstart = XMLoadFloat4(&rayData.start);
	vstart = XMVector3TransformCoord(vstart, wInv); //transformcoordはw要素を無視してくれるらしい
	//④（始点から方向ベクトルをちょい伸ばした先）通過点（②）に①をかける
	vpass = XMVector3TransformCoord(vpass, wInv);
	//⑤rayData.dirを③から④に向かうベクトルにする（引き算）
	vpass = 
	XMStoreFloat4(&rayData.dir, vpass);

	//指定したモデル番号のFBXにレイキャスト
	modelList[hModel]->pfbx_->RayCast(rayData);
}

