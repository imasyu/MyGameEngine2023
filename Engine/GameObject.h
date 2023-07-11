#pragma once
#include <list>
#include <string>
#include "Transform.h"
#include "Direct3D.h"

using std::string;
using std::list;

class GameObject
{
protected:
	list<GameObject*>         childList_;
	Transform	              transform_;
	GameObject*	              pParent_;
	string	                  objectName_;
	bool                      Dead_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() =0;
	virtual void Update() =0;
	virtual void Draw() =0;
	virtual void Release() =0;
	void KillMe();
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();
	template<class T>
	void Instantiate(GameObject* parent)
	{
		T* pObject;
		pObject = new T(parent);
		pObject->Initialize();
		parent->childList_.push_back(pObject);
	}
};