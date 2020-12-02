#include "GamePlay.h"
#include"Title.h"

GamePlay::GamePlay()
{
	
}

GamePlay::~GamePlay()
{
	delete objM;//重要
}

void GamePlay::StartScene()
{
	camera->SetEye(Vector3(0, 0, -100.0f));
	objM = new ObjectManager();
	objM->Claer();
	objM->Add(new Player(Vector3(0, 0, 0), Vector3(0, 0, 0), objM, BaseScene::mModel,BaseScene::mSprite,BaseScene::mModel));
	objM->Add(new Enemy(Vector3(0, 0, 0), Vector3(0, 0, 0), objM, BaseScene::mModel,BaseScene::mParticle));
	x = 0.0f;
	y = 4.0f;
	z = -60.0f;
}

void GamePlay::UpdateScene()
{
	
	objM->Update();
	//x+=0.2f;
}

void GamePlay::DrawScene()
{
	BaseScene::mModel->DrawModel(0, Vector3(0, 5.0f, -90.0f), Vector3(0, 0, 0),Vector3(1,1,1));//地面
	//BaseScene::mModel->DrawModel(1, Vector3(0.0f, 5.0f, -90.0f), Vector3(0, 0, 0),Vector3(1,1,1));//天球
}
