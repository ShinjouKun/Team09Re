#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include"Model.h"
#include"Sprite.h"
#include<memory>
#include"Camera.h"
class Player :public BaseObject
{
public:
	Player(Vector3 pos,
		Vector3 angle, 
		ObjectManager* obj,
		std::shared_ptr<Model>m,
		std::shared_ptr<Sprite>s, 
		std::shared_ptr<Model>bm);
	~Player();

	void Shot();

	// BaseObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void Hit(BaseObject & other) override;

private:
	void WeaponChange();

	void LimetAngle(float limet);

private:
	ObjectManager* objM;
	std::shared_ptr<Model>playerModel;
	std::shared_ptr<Model>bulletModel;
	std::shared_ptr<Sprite>hitSprite;
	Camera* camera;
	float r;
	Vector3 TargetPos;

	//int _limitAngle; //Å@å¿äEäp

	float xtilt;
	float targetX, targetY;
	int mode;
	int time;

	int HP;
};