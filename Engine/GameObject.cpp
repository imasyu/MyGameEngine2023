#include "GameObject.h"


GameObject::GameObject()
	:pParent_(nullptr), Dead_(false)
{

}

GameObject::GameObject(GameObject* parent, const std::string& name)
	: Dead_(false), pParent_(parent), objectName_(name)
{
	if (parent != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::KillMe()
{
	Dead_ = true;
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->DrawSub();
}

void GameObject::UpdateSub()
{
	Update();

	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->Dead_ == true)
		{
			(*itr)->ReleaseSub();
			SAFE_DELETE(*itr);//�������g������
			itr = childList_.erase(itr);//���X�g������폜
		}
		else
		{

			itr++;
		}
	}
}

void GameObject::ReleaseSub()
{
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->ReleaseSub();//*itr�̃����[�X���Ă�
		SAFE_DELETE(*itr);//*itr���̂�����
	}
	Release();
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

void GameObject::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_)
	{
		return(this); //������_objName�̃I�u�W�F�N�g�������I
	}
	else
	{
		//for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		for(auto itr: childList_)
		{
			GameObject* obj = itr->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

/// <summary>
/// �ċA�Ăяo����RootJob��T���Ă��̃A�h���X��Ԃ��֐�
/// </summary>
/// <returns>RootJob�̃A�h���X(GameObject * �^)</returns>
GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	//�������猩�āA���[�g�W���u��T���āA���̃��[�g�W���u����S���̎q�����ǂ���_objName��T���I
	GameObject* rootJob = GetRootJob();
	GameObject* result = rootJob->FindChildObject(_objName);
	return(result);
}
