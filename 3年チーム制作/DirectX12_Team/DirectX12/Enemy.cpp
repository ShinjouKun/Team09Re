#include "Enemy.h"

Enemy::Enemy(Vector3 pos, Vector3 angle, ObjectManager * obj, std::shared_ptr<Model> m, std::shared_ptr<ParticleEmitter> p):
	enemyModel(m),enemyP(p)
{
	position = pos;
	objM = obj;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	position = Vector3(0.0f, 2.0f, -50.0f);
	death = false;
	objType = ObjectType::ENEMY;
	enemyModel->SetModelAncPoint(3, Vector3(-0.3f, 0.5f, 1.85f), Vector3(0, r, 0));//‹°‚ç‚­^‚ñ’†
	r = 0.0f;
	SphereSize = 1.0f;
}

void Enemy::Update()
{
	enemyP->SetPos(Vector3(position.x, position.y, position.z));
}

void Enemy::Rend()
{

	enemyModel->DrawModel(3, (Vector3(position.x, position.y, position.z)), Vector3(0, r, 0),Vector3(1,1,1));
}

void Enemy::Hit(BaseObject & other)
{
	if (other.GetType() == ObjectType::BULLET)
	{
		enemyP->Update();
	}
}
