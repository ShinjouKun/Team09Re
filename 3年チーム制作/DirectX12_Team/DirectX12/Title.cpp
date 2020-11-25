#include "Title.h"
#include"GamePlay.h"
#include"Sprite.h"
Title::Title():BaseScene()
{
}

Title::~Title()
{

}

void Title::StartScene()
{
	camera->SetEye(Vector3(0, 0, -300.0f));
}

void Title::UpdateScene()
{
	if (Input::KeyDown(DIK_SPACE))
	{
		NextScene(std::make_shared<GamePlay>());
	}
	camera->SetTarget(Vector3(0,0,100.0f));
}

void Title::DrawScene()
{
	BaseScene::mSprite->Draw3D(0, Vector3(0, 0, 0), Vector3(0, 0, 0));
}
