#pragma once
#include<memory>
#include"Sprite.h"
#include"Model.h"
#include"Particle.h"
#include"BaseScene.h"

class SceneManager
{
public:
	SceneManager(std::shared_ptr<Sprite> sprite,
	std::shared_ptr<Model>model,
		std::shared_ptr<ParticleEmitter>particle);
	~SceneManager();
	void Update();
	void Draw()const;
private:
	void SetSpriteToScene();
	void SetModelToScene();
	void SetParticleToScene();

	std::shared_ptr<Sprite>mSprite;
	std::shared_ptr<Model>mModel;
	std::shared_ptr<ParticleEmitter>mParticle;

	std::shared_ptr<BaseScene>mNowScene;
};
