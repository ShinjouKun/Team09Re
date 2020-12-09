#include "Player.h"
#include "Input.h"
#include"Bullet.h"
#include"Easing.h"


Player::Player(Vector3 pos, Vector3 ang, ObjectManager * obj,std::shared_ptr<Model>m,std::shared_ptr<Sprite>s, std::shared_ptr<Model>bm)
	:playerModel(m),hitSprite(s),bulletModel(bm)
{
	position = pos;
	objM = obj;
	angle = ang;
}

Player::~Player()
{
}

void Player::Shot()
{
	objM->Add(new Bullet(Vector3(position.x,position.y,position.z),
		Vector3(angle.x, angle.y, angle.z), objM, bulletModel));
	/*objM->Add(new Bullet(Vector3(position.x,position.y,position.z),
		Vector3(angle.x, angle.y, 0.0f), objM, bulletModel));*/

}

void Player::Init()
{
	position = Vector3(0.0f, 4.0f, -90.0f);
	death = false;
	objType = ObjectType::PLAYER;
	
	playerModel->SetModelAncPoint(2, Vector3(0.0f, -1.0f, 0.0f),
		Vector3(angle.x, angle.y, 0));//恐らく真ん中
	
	hitSprite->SetSize(1, Vector2(1, 1));
	angle.y = 0.0f;
	angle.x = 0.0f;
	SphereSize = 1.0f;
	xtilt = 0;
	targetX = 0.0f;
	targetY = 0.0f;
	mode = 0;
	time = 0;

	camera->eye = Vector3(0,0,-100);
	TargetPos = Vector3(targetX, targetY, position.z + 50.0f);//弾の飛ぶ先を注視

}

void Player::Update()
{

	//キー押し処理
	if (Input::KeyState(DIK_UP))
	{
		position.z += 0.2f;
		camera->CameraMoveEyeVector(Vector3(0,0,0.2f));		
	}
	if (Input::KeyState(DIK_DOWN))
	{
		position.z-= 0.2f;
		camera->CameraMoveEyeVector(Vector3(0, 0, -0.2f));		
	}
	if (Input::KeyState(DIK_RIGHT))
	{
		//position.x += 0.4f;
		camera->CameraMoveEyeVector(Vector3(0.4f, 0, 0));
	}
	if (Input::KeyState(DIK_LEFT))
	{
		//position.x -= 0.4f;
		camera->CameraMoveEyeVector(Vector3(-0.4f, 0, 0));
	}
	if (Input::KeyState(DIK_1))
	{
		angle.y -= 3.0f;
	}
	if (Input::KeyState(DIK_2))
	{
		angle.y += 3.0f;
	}
	if (Input::KeyState(DIK_3))
	{
		angle.x -= 3.0f;
	}
	if (Input::KeyState(DIK_4))
	{
		angle.x += 3.0f;
	}
	if (Input::KeyState(DIK_SPACE))
	{
		Shot();
	}

	/*　ゲームパッド　*/
	//　左スティック()	
	if (Input::pad_data.lX < 0) {

		angle.y += 4.0f;


	}
	if (Input::pad_data.lX > 0) {

		angle.y -= 4.0f;


	}
	if (Input::pad_data.lY < 0) {

		angle.x -= 4.0f;

	}
	if (Input::pad_data.lY > 0) {

		angle.x += 4.0f;
	}

	//　右スティック
	if (mode == 0)
	{
		if (Input::pad_data.lZ < 0) {
			camera->CameraMoveEyeVector({ -0.2f,0,0 });

		}
		if (Input::pad_data.lZ > 0) {
			camera->CameraMoveEyeVector({ 0.2f,0,0 });

		}
		if (Input::pad_data.lRz < 0) {
			camera->CameraMoveEyeVector({ 0,0.2f,0 });

		}
		if (Input::pad_data.lRz > 0) {
			camera->CameraMoveEyeVector({ 0,-0.2f,0 });

		}

	}
	//　ショット
	if (Input::PushButton(BUTTON_B)) {
		Shot();
	}

	/*　弾チェンジ　*/
	WeaponChange();
	
	{
		if (Input::PushButton(BUTTON_Y)) {

			if (time < 20)time++;
			camera->eye = Easing::ease_out_expo(time,
				camera->eye, Vector3(0, 0, -95.0f) - camera->eye, 200);

			mode = 1;
		}
		else {

			camera->eye.z = -100.0f;
			camera->eye.y = 0.0f;
			mode = 0;
		}
	
}

	//if (time < 500)time++;
	//position = Easing::ease_in_sine(time, position,Vector3(0,4,-50.0f) - position, 800);

	TargetPos = Vector3(targetX, targetY, position.z + 50.0f);//弾の飛ぶ先を注視
	camera->SetTarget(Vector3(TargetPos.x, TargetPos.y, TargetPos.z));
	camera->SetEye(Vector3(position.x, position.y, position.z - 10.0f));


}

void Player::Rend()
{

	playerModel->DrawModel(2,(Vector3(position.x,position.y+4,position.z)), 
		Vector3(angle.x,angle.y,angle.z),Vector3(1.0f,1.0f,1.0f));
	//hitSprite->Draw3D(1, Vector3(0, 0, 0.0f), Vector3(0, 0, 0));
	

	playerModel->DrawModel(2, (Vector3(position.x, position.y - 2.0f, position.z)), Vector3(-angle.x, angle.y, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	playerModel->DrawModel(5, (Vector3(position.x, position.y, position.z)), Vector3(0.0f, angle.y, angle.z), Vector3(1.0f, 1.0f, 1.0f));
	playerModel->DrawModel(6, (Vector3(position.x, position.y, position.z)), Vector3(0.0f, 0.0f, angle.z), Vector3(1.0f, 1.0f, 1.0f));
}

void Player::Hit(BaseObject & other)
{

}

void Player::WeaponChange()
{
	//　弾の切り替え
	if (Input::TriggerButton(BUTTON_R1)) {

	}
	if (Input::TriggerButton(BUTTON_L1)) {

	}

}
