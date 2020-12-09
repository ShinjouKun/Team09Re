#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include"Model.h"
#include"Particle.h"
#include<memory>
class Enemy :public BaseObject
{
public:
	Enemy(Vector3 pos,
		Vector3 angle, 
		ObjectManager* obj,
		std::shared_ptr<Model>m, 
		std::shared_ptr<ParticleEmitter>p);
	~Enemy();

private:


	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void Hit(BaseObject & other) override;
private:
	ObjectManager* objM;
	std::shared_ptr<Model>enemyModel;
	std::shared_ptr<ParticleEmitter>enemyP;
	float r;
	

};
