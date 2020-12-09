#include"Windows.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
#include"wrl.h"

using namespace Microsoft::WRL;
//���w���C�u����
#include"DirectXMath.h"
using namespace DirectX;
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//�C���v�b�g
#include"Input.h"
//������
#include"DirectXManager.h"
//�X�v���C�g�`��
#include"Sprite.h"
//�p�[�e�B�N��
#include"Particle.h"
//���f��
#include"Model.h"
//�f�o�b�O�X�v���C�g
#include"DebugText.h"
//��
#include"Sound.h"
//�E�B���h�E
#include"Window.h"
//�J����
#include"Camera.h"
//�V�[��
#include"SceneManager.h"
std::unique_ptr<SceneManager>mScene;//���̃N���X���������|�C���^

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
//�f�o�b�O
ComPtr<ID3D12Debug> debugController;

void Debug();//�f�o�b�O

//Main�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E�N���X
	Window* window = nullptr;
	window = new Window();
	window->CreateGameWindow();
	Debug();//�f�o�b�O�p
	//DXManager
	DirectXManager* dxManager = new DirectXManager();
	dxManager->Init(window);
	//�J����
	Camera* camera = new Camera();
	//camera->SetEye(Vector3(0, 0, 0.0f));
	//�X�v���C�g
	//Sprite* sprite = new Sprite(dxManager->Dev(),dxManager->CmdList());
	std::shared_ptr<Sprite>sprite = std::make_shared<Sprite>(dxManager->Dev(), dxManager->CmdList());
	//�p�[�e�B�N��
	//std::shared_ptr<Particle>paricle = std::make_shared<Particle>(dxManager->Dev(), dxManager->CmdList());
	std::shared_ptr<ParticleEmitter>paricle = std::make_shared<ParticleEmitter>(dxManager->Dev(), dxManager->CmdList());
	//Particle* particle = new Particle(dxManager->Dev(), dxManager->CmdList());
	//���f��
	std::shared_ptr<Model>model = std::make_shared<Model>(dxManager->Dev(), dxManager->CmdList());
	//Model* model = new Model(dxManager->Dev(), dxManager->CmdList());
	
	//�T�E���h
	Sound* sound = new Sound();
	sound->LoadSE("Resouse/endingse.wav");
	sound->Play("Resouse/endingse.wav");
	//�ǂݍ���
	model->LoadModel(L"Resouse/ground.obj", 0);
	model->LoadModel(L"Resouse/skydome.obj", 1);
	model->LoadModel(L"Resouse/taihou.obj", 2);
	model->LoadModel(L"Resouse/BoxTank.obj", 3);
	model->LoadModel(L"Resouse/Bullet.obj", 4);
	model->LoadModel(L"Resouse/yoko.obj", 5);
	model->LoadModel(L"Resouse/daiza.obj", 6);
	sprite->LoadTex(L"Resouse/TaimaPark.jpg");
	sprite->CreateSpliteDate3D(L"Resouse/TaimaPark.jpg", 0, VertType::PLANE);
	sprite->LoadTex(L"Resouse/carsoll.png");
	sprite->CreateSpliteDate3D(L"Resouse/carsoll.png",1,VertType::PLANE);
	paricle->Load();
	
	Input* input = new Input();//�C���v�b�g�C���X�^���X����
	input->Init(window->GetHWND());//�C���v�b�g������
	input->InitGamepad(window->GetHWND());
	//�V�[��
	mScene = std::make_unique<SceneManager>(sprite, model, paricle);
		//���[�v����
	float x = 0;
	float y = 2.0f;
	float z = -90.0f;
	float r = 180.0f;
	
	while (true)
	{
		if (input->KeyDown(DIK_ESCAPE))
		{
			return 0;
		}


		if (window->ProcesssMessage()) { break; }//���b�Z�[�W����
		dxManager->Begin();
		//�`��
#pragma region �L�[����
		input->Update();//input
		input->UpdateGamepad();
		//�L�[�����܂�
#pragma endregion
		//�V�[��
		mScene->Update();
		mScene->Draw();


		dxManager->End();
	}


	window->DeleateGameWindow();//�Q�[��window�j��
	delete input;
	delete dxManager;
	delete camera;
	delete sound;
	return 0;
}


void Debug()
{
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}
