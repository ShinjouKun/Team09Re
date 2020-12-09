#include"Windows.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
#include"wrl.h"

using namespace Microsoft::WRL;
//数学ライブラリ
#include"DirectXMath.h"
using namespace DirectX;
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//インプット
#include"Input.h"
//初期化
#include"DirectXManager.h"
//スプライト描画
#include"Sprite.h"
//パーティクル
#include"Particle.h"
//モデル
#include"Model.h"
//デバッグスプライト
#include"DebugText.h"
//音
#include"Sound.h"
//ウィンドウ
#include"Window.h"
//カメラ
#include"Camera.h"
//シーン
#include"SceneManager.h"
std::unique_ptr<SceneManager>mScene;//このクラスだけが持つポインタ

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
//デバッグ
ComPtr<ID3D12Debug> debugController;

void Debug();//デバッグ

//Main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウクラス
	Window* window = nullptr;
	window = new Window();
	window->CreateGameWindow();
	Debug();//デバッグ用
	//DXManager
	DirectXManager* dxManager = new DirectXManager();
	dxManager->Init(window);
	//カメラ
	Camera* camera = new Camera();
	//camera->SetEye(Vector3(0, 0, 0.0f));
	//スプライト
	//Sprite* sprite = new Sprite(dxManager->Dev(),dxManager->CmdList());
	std::shared_ptr<Sprite>sprite = std::make_shared<Sprite>(dxManager->Dev(), dxManager->CmdList());
	//パーティクル
	//std::shared_ptr<Particle>paricle = std::make_shared<Particle>(dxManager->Dev(), dxManager->CmdList());
	std::shared_ptr<ParticleEmitter>paricle = std::make_shared<ParticleEmitter>(dxManager->Dev(), dxManager->CmdList());
	//Particle* particle = new Particle(dxManager->Dev(), dxManager->CmdList());
	//モデル
	std::shared_ptr<Model>model = std::make_shared<Model>(dxManager->Dev(), dxManager->CmdList());
	//Model* model = new Model(dxManager->Dev(), dxManager->CmdList());
	
	//サウンド
	Sound* sound = new Sound();
	sound->LoadSE("Resouse/endingse.wav");
	sound->Play("Resouse/endingse.wav");
	//読み込み
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
	
	Input* input = new Input();//インプットインスタンス生成
	input->Init(window->GetHWND());//インプット初期化
	input->InitGamepad(window->GetHWND());
	//シーン
	mScene = std::make_unique<SceneManager>(sprite, model, paricle);
		//ループ処理
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


		if (window->ProcesssMessage()) { break; }//メッセージ処理
		dxManager->Begin();
		//描画
#pragma region キー入力
		input->Update();//input
		input->UpdateGamepad();
		//キーここまで
#pragma endregion
		//シーン
		mScene->Update();
		mScene->Draw();


		dxManager->End();
	}


	window->DeleateGameWindow();//ゲームwindow破棄
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
