#pragma once
#include"BaseScene.h"
#include"Input.h"
#include<memory>
#include"Camera.h"
class Title:public BaseScene
{
public:
	Title();
	~Title();
	
private:

	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;
};
