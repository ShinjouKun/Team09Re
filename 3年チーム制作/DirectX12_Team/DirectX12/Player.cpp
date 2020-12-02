#include "Player.h"
#include "Input.h"
#include"Bullet.h"

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
	objM->Add(new Bullet(Vector3(position.x,position.y,position.z), Vector3(angle.x, angle.y, 0.0f), objM, bulletModel));
}

void Player::Init()
{
	position = Vector3(0.0f, 4.0f, -90.0f);
	death = false;
	objType = ObjectType::PLAYER;
	
	playerModel->SetModelAncPoint(2, Vector3(0.0f, -1.0f, 0.0f), Vector3(angle.x, angle.y, 0));//恐らく真ん中
	
	hitSprite->SetSize(1, Vector2(1, 1));
	angle.y = 0.0f;
	angle.x = 0.0f;
	SphereSize = 1.0f;
}

void Player::Update()
{
	TargetPos = Vector3(0.0f, 0.0f, position.z + 50.0f);//弾の飛ぶ先を注視
	//キー押し処理
	if (Input::KeyState(DIK_UP))
	{
		position.y += 0.2f;
		camera->CameraMoveEyeVector({0,0.2f,0});
	}
	if (Input::KeyState(DIK_DOWN))
	{
		position.y -= 0.2f;
		camera->CameraMoveEyeVector({ 0,-0.2f,0 });
	}

	if (Input::KeyState(DIK_RIGHT))
	{
		position.x += 0.4f;
		camera->CameraMoveEyeVector({ 0.4f,0,0 });
	}
	if (Input::KeyState(DIK_LEFT))
	{
		position.x -= 0.4f;
		camera->CameraMoveEyeVector({ -0.4f,0,0 });
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
}

void Player::Rend()
{
	playerModel->DrawModel(2, (Vector3(position.x, position.y - 2.0f, position.z)), Vector3(-angle.x, angle.y, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	playerModel->DrawModel(5, (Vector3(position.x, position.y, position.z)), Vector3(0.0f, angle.y, angle.z), Vector3(1.0f, 1.0f, 1.0f));
	playerModel->DrawModel(6, (Vector3(position.x, position.y, position.z)), Vector3(0.0f, 0.0f, angle.z), Vector3(1.0f, 1.0f, 1.0f));
	camera->SetTarget(Vector3(TargetPos.x, TargetPos.y, TargetPos.z));
}

void Player::Hit(BaseObject & other)
{

}
