#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
#include"Model.h"
class Bullet:public BaseObject
{
public:
	Bullet(Vector3 pos, Vector3 angle, ObjectManager* obj, std::shared_ptr<Model>m);
	~Bullet();

private:


	// BaseObject を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void Hit(BaseObject & other) override;
	ObjectManager* objM;
	std::shared_ptr<Model>BulletModel;
	Vector3 TargetPos;

};
