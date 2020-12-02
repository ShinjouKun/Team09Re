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
}

void Player::Init()
{
	position = Vector3(0.0f, 4.0f, -90.0f);
	death = false;
	objType = ObjectType::PLAYER;
	
	playerModel->SetModelAncPoint(2, Vector3(0.0f, 0.5f, 1.85f),Vector3(0,angle.y,0));//恐らく真ん中
	
	hitSprite->SetSize(1, Vector2(1, 1));
	angle.y = 0.0f;
	SphereSize = 1.0f;
	xtilt = 0;
	targetX = 0.0f;
	targetY = 0.0f;
	mode = 0;
	time = 0;
}

void Player::Update()
{
	TargetPos = Vector3(targetX, targetY, position.z + 50.0f);//弾の飛ぶ先を注視

	//キー押し処理
	if (Input::KeyState(DIK_UP))
	{
		position.z += 0.2f;
		camera->CameraMoveEyeVector({0,0,0.2f});
		
	}
	if (Input::KeyState(DIK_DOWN))
	{
		position.z-= 0.2f;
		camera->CameraMoveEyeVector({ 0,0,-0.2f});
		
	}

	if (Input::KeyState(DIK_RIGHT))
	{
		position.x += 0.2f;
		camera->CameraMoveEyeVector({ 0.2f,0,0 });
	}
	if (Input::KeyState(DIK_LEFT))
	{
		position.x -= 0.2f;
		camera->CameraMoveEyeVector({ -0.2f,0,0 });
	}
	if (Input::KeyState(DIK_1))
	{
		angle.y -= 2.0f;
	}
	if (Input::KeyState(DIK_2))
	{
		angle.y += 2.0f;
	}
	if (Input::KeyState(DIK_SPACE))
	{
		Shot();
	}

	/*　ゲームパッド　*/
	//　左スティック()
	
	if (Input::pad_data.lX < 0) {

		targetX -= 2.0f;
		//camera->CameraMoveEyeVector({ -0.2f,0,0 });

	}
	if (Input::pad_data.lX > 0) {

		targetX += 2.0f;

		//camera->CameraMoveEyeVector({ +0.2f,0,0 });

	}
	if (Input::pad_data.lY < 0) {

		targetY += 2.0f;
		//camera->CameraMoveEyeVector({ 0,0.2f,0 });

	}
	if (Input::pad_data.lY > 0) {

		targetY -= 2.0f;
		//camera->CameraMoveEyeVector({ 0,-0.2f,0 });

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
	if (Input::PushButton(BUTTON_A)) {
		Shot();
	}

	
	if (Input::PushButton(BUTTON_Y)) {
		
		if (time < 20)time++;
		camera->eye = Easing::ease_out_expo(time,
			camera->eye, Vector3(0, 0, -95.0f) - camera->eye, 200);

		

		mode = 1;
	}
	else
	{
		//if (time > 0)time--;

		//camera->eye.z = Easing::ease_out_expo(time,
		//	camera->eye.z, -100.0f - camera->eye.z, 200);

		camera->eye.z = -100.0f;
		camera->eye.y = 0.0f;
		mode = 0;
	}

	/*if (time < 200)time++;
	position = Easing::ease_out_expo(time, position,Vector3(0,4,-50.0f) - position, 300);

	camera->eye = Easing::ease_out_expo(time, camera->eye, 
		Vector3(0, 0, -50.0) - camera->eye, 300);
*/
}

void Player::Rend()
{
	playerModel->DrawModel(2,(Vector3(position.x,position.y,position.z)), Vector3(angle.x,angle.y,angle.z),Vector3(1.0f,1.0f,1.0f));
	//hitSprite->Draw3D(1, Vector3(0, 0, 0.0f), Vector3(0, 0, 0));
	//camera->SetEye(Vector3(0, 0 , position.z - 10.0f));
	camera->SetTarget(Vector3(TargetPos.x, TargetPos.y, TargetPos.z));
}

void Player::Hit(BaseObject & other)
{
	if (other.GetType() == ObjectType::ENEMY)
	{
		
	}
}

void Player::WeaponChange()
{
	//　弾の切り替え
	if (Input::TriggerButton(BUTTON_R1)) {

	}
	if (Input::TriggerButton(BUTTON_L1)) {

	}

}
