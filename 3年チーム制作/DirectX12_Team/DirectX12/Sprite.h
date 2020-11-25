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

enum VertType
{
	CUBE,//箱
	PLANE,//板
	SPHERE,//球
	NONE,
};

struct ConstBuffDate
{
	Vector4 color;
	Matrix4 mat;
};
//2D用
struct VertexUV
{
	Vector3 pos;
	Vector2 uv;
};
//3D用
struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};
struct DirectVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

//2D用
struct SpriteData
{
	//GPUリソースの生成
	ComPtr<ID3D12Resource> vertBuff;
    //定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Matrix4 matWorld;//ワールド行列
	Vector4 color;
	UINT texNum;//テクスチャ番号
	Vector2 texSize;//テクスチャのサイズ
	Vector2 ancPoint;//アンカーポイント
	Vector4 texUV;//テクスチャの切り取り

};
//3D要
struct SpriteData3D
{
	std::vector<Vertex>vertex;
	std::vector<unsigned short>index;
	//GPUリソースの生成
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource>indexBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	Matrix4 matWorld;//ワールド行列
	Vector4 color;
	UINT texNum;//テクスチャ番号
	Vector3 ancPoint3D;
};
class Sprite
{
public:
	Sprite(ID3D12Device* device,ID3D12GraphicsCommandList*commandList);
	~Sprite();
	void Init();
	void Update();
	void CreateBuff();
	void CreateBuff3D();
	void CreateVert();
	void CreateVertCube();
	void CreateVertSphere();
	void CreateVertPlane();
	void CreateNormal();

	HRESULT InitGraphicPipeLine();
	HRESULT InitGraphicPipeLine3D();
	void LoadTex(const wchar_t* filename);
	
	void CreateSpliteDate(const wchar_t* filename,int num);
	void CreateSpliteDate3D(const wchar_t* filename, int num,VertType type);
	void SetAncPoint(int num, Vector2 ancPoint);//アンカーのセット
	void SetSize(int num, Vector2 size);
	void SetUV(int num, float left, float top, float right, float botom, bool inversionX = false, bool inversionY = false);
	void SetColor(int num, Vector4 col);
	void Draw2D(int num,const Vector3& pos,float rotare = 0.0f);
	void Draw3D(int num, const Vector3& pos,const Vector3& angle);
	void Draw3DBill(int num, const Vector3& pos, float rotate = 0.0f);//全方向ビルボード
	void Draw3DBillY(int num, const Vector3& pos, float rotate = 0.0f);//Y軸ビルボード
	//当たり判定確認用
	void DebugBox(int num,const Vector3& pos, const Vector3& angle);
	void DebugSphere(int num,const Vector3& pos);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;

	SpriteData data;
	std::unordered_map<int ,SpriteData>dataList;//スプライトのデータリスト
	std::unordered_map<const wchar_t*, ComPtr<ID3D12Resource>>texList;//テクスチャリスト FixMe
	std::unordered_map<const wchar_t*, int>countList;//テクスチャの名前からINTで検索する
	std::vector<VertexUV> vertex;
	ComPtr<ID3D12DescriptorHeap>spriteDescHeap;
	ComPtr<ID3D12PipelineState>spritePipeLineState;//パイプラインステート
	ComPtr<ID3D12RootSignature>spriteRootSignature;//ルートシグネスチャ
	ConstBuffDate*constMap;//定数バッファにデータを転送
	//３D用
	SpriteData3D data3d;
	std::unordered_map<int, SpriteData3D>dataList3d;//スプライトのデータリスト
	ComPtr<ID3D12PipelineState>PipeLineState3d;//パイプラインステート
	ComPtr<ID3D12RootSignature>RootSignature3d;//ルートシグネスチャ
	Matrix4 matProjection3d;//射影変換行列
	
   //シェーダファイルの読み込みとコンパイル
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	ComPtr<ID3DBlob>rootSigBlob;

	Matrix4 matProjection;//射影変換行列

	int count;//テクスチャ番号
	

	HRESULT result;
	
};
