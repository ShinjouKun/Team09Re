#include "SceneManager.h"
#include"Title.h"

SceneManager::SceneManager(std::shared_ptr<Sprite> sprite, std::shared_ptr<Model> model, std::shared_ptr<ParticleEmitter> particle)
	:mSprite(sprite),mModel(model),mParticle(particle),
	mNowScene(std::make_shared<Title>())//Å‰‚Íƒ^ƒCƒgƒ‹
{
	SetSpriteToScene();
	SetModelToScene();
	SetParticleToScene();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	mNowScene->Update();
	auto next = mNowScene->GetNextScene();
	if (next)
	{
		mNowScene = next;
		SetSpriteToScene();
		SetModelToScene();
		SetParticleToScene();
	}
}

void SceneManager::Draw() const
{
	mNowScene->Draw();
}

void SceneManager::SetSpriteToScene()
{
	mNowScene->SetSprite(mSprite);
}

void SceneManager::SetModelToScene()
{
	mNowScene->SetModel(mModel);
}

void SceneManager::SetParticleToScene()
{
	mNowScene->SetParticle(mParticle);
}
