#include "Model.h"
#include "Engine/Direct3D.h"

struct ModelData
{
	Fbx* pfbx_;
	Transform transform_;
	std::string filename_;
	//�g�����X�t�H�[��
};
//���f���̃|�C���^���Ԃ�����ł����x�N�^
std::vector<ModelData*> modelList;

int Model::Load(std::string fileName)
{
	//���ł���
	//modelList�ɂԂ�����
	//���Ă񂾃��f���̃��f���ԍ���Ԃ��I
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = fileName;
	pData->pfbx_ = nullptr;

	//�t�@�C���l�[����������������A�ǂ܂Ȃ�
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
	//���f���ԍ��́AmodelList�̃C���f�b�N�X
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	//���f���ԍ��́AmodelList�̃C���f�b�N�X
	modelList[hModel]->pfbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false; //�Q�Ƃ���Ă���H
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
