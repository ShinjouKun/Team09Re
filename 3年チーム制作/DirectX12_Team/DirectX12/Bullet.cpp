#include "Bullet.h"
#include"Matrix4.h"
Bullet::Bullet(Vector3 pos, Vector3 ang, ObjectManager* obj, std::shared_ptr<Model>m):
	BulletModel(m)
{
	position = pos;
	objM = obj;
	angle = ang;
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	death = false;
	objType = ObjectType::BULLET;
	SphereSize = 2.0f;
	BulletModel->SetModelAncPoint(4, Vector3(0.4f, 0.5f, 1.85f),
		Vector3(angle.x, angle.y, angle.z));//‹°‚ç‚­^‚ñ’†


}

void Bullet::Update()
{
	velocity = Vector3(0, 0, 0);
	velocity = RotateX(-angle.x);
	velocity = RotateY(-angle.y - 90.0f)+velocity*1.5f;
	
	position += velocity;
	if (position.z > 0)
	{
		death = true;
	}
}

void Bullet::Rend()
{
	BulletModel->DrawModel(4,Vector3(position.x,position.y,position.z),Vector3(0,angle.y,0),Vector3(1,1,1));
}

void Bullet::Hit(BaseObject & other)
{
	if (other.GetType() == ObjectType::ENEMY)
	{
		death = true;
	}
}
