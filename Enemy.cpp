#include "Enemy.h"
#include "Engine/SphireCollider.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), pFbx(nullptr)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Assets/Oden.fbx");
	transform_.position_.z = 20.0f;

	SphireCollider* col = new SphireCollider(1.2f);
	AddCollider(col);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	pFbx->Draw(transform_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//Childoden‚É‚Ô‚Â‚©‚Á‚½‚Æ‚«
	KillMe();
	pTarget->KillMe();

}

