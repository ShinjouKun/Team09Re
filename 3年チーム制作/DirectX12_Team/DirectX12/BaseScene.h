#pragma once
#include<memory>
#include"Sprite.h"
#include"Model.h"
#include"Particle.h"

class BaseScene
{
protected:
	BaseScene();
	virtual~BaseScene();
	std::shared_ptr<Sprite>mSprite;
	std::shared_ptr<Model>mModel;
	std::shared_ptr<ParticleEmitter>mParticle;
public:
	virtual void StartScene() = 0;
	void Update();
	virtual void UpdateScene() = 0;
	void Draw();
	virtual void DrawScene() = 0;
	void NextScene(std::shared_ptr<BaseScene>next);
	std::shared_ptr<BaseScene>GetNextScene()const;
	void SetSprite(std::shared_ptr<Sprite>sprite);
	void SetModel(std::shared_ptr<Model>model);
	void SetParticle(std::shared_ptr<ParticleEmitter>particle);
private:
	std::shared_ptr<BaseScene>mNext;
	void Start();
	bool isInit;//èâä˙âª
};
