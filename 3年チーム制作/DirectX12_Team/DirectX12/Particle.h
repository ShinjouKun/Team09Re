#pragma once
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include<memory>
#include<iostream>
#include<unordered_map>
#include"wrl.h"
#include"vector"
#include<map>
#include<forward_list>
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//ウィンドウ
#include"Window.h"

//数学ライブラリ
#include"DirectXMath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"


using namespace DirectX;
using namespace Microsoft::WRL;


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
struct ConstBuffData
{
	Matrix4 mat;
	Matrix4 matBill;
};
struct VertexP
{
	Vector3 pos;//ポジション
	Vector4 color;
	float scale =1.0f;//サイズ
};
struct DirectVertexP
{
	XMFLOAT3 pos;
};
struct ParticleData
{
	std::vector<VertexP>vertex;
	//GPUリソースの生成
	ComPtr<ID3D12Resource> vertBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//Matrix4 matWorld;//ワールド行列
	UINT texNum;//テクスチャ番号
	Vector3 ancPoint3D;
};
//パーティクル一粒の構造体
struct ParticleStateus
{
	Vector3 pos;//ポジション
	Vector3 vel;//速度
	Vector3 acc;//加速度
	Vector4 color;
	float scale = 1.0f;//サイズ
	//初期スケール
	float sScale = 1.0f;
	//最終スケール
	float eScale = 0.0f;
	int frame = 0;//現在フレーム
	int numFrame = 0;//終了フレーム
};

class Particle
{
public:
	

	Particle(ID3D12Device* device, ID3D12GraphicsCommandList*commandList);
	~Particle();
	void Init();
	void Update();
	void CreateBuff();
	void CreatePlane();//パーティクル用のポリゴン
	HRESULT InitGraphicPipeLine();
	void LoadTex(const wchar_t* filename);
	void CreateParticleData(const wchar_t* filename,const int& num);
	void DrawParticleBill(const int num);//全方向ビルボード
	void DrawParticleBillY(const int& num, const Vector3& pos);//Y軸ビルボード

	//リストへ追加
	void Add(int life, Vector3 pos, Vector3 vel, Vector3 acc,float sScale,float eScale,Vector4 color);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;
	ConstBuffData*constMap;//定数バッファにデータを転送
	std::unordered_map<const wchar_t*, ComPtr<ID3D12Resource>>texList;//テクスチャリスト FixMe
	std::unordered_map<const wchar_t*, int>countList;//テクスチャの名前からINTで検索する
	ParticleData dataP;
	ComPtr<ID3D12DescriptorHeap>DescHeap;
	std::unordered_map<int, ParticleData>dataListP;//スプライトのデータリスト
	ComPtr<ID3D12PipelineState>PipeLineStateP;//パイプラインステート
	ComPtr<ID3D12RootSignature>RootSignatureP;//ルートシグネスチャ
	Matrix4 matProjectionP;//射影変換行列
	//シェーダファイルの読み込みとコンパイル
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>gsBlob;//ジオメトリ用
	ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	ComPtr<ID3DBlob>rootSigBlob;
	int count;//テクスチャ番号
	HRESULT result;
	//パーティクルリスト
	std::forward_list<ParticleStateus>particles;
	int vertCount = 1024;
};

class ParticleEmitter
{
private:
	Vector3 pos;
	Particle* particle;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
public:
	ParticleEmitter(ID3D12Device* device, ID3D12GraphicsCommandList*commandList);
	~ParticleEmitter();
	void Load();
	void Update();
	void SetPos(Vector3 pos);

};
