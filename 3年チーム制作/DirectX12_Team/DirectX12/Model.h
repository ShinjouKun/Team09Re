#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<memory>
#include<iostream>
#include<unordered_map>
#include<wrl.h>
#include<vector>
#include<map>
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"

//計算用
#include<DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

//マテリアル用
struct OBJMat
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	std::string TexKeyWord;
	std::string TextureName;
};
//定数バッファ
struct ModelBuffDate
{
	Vector4 color;
	Matrix4 mat;
	XMFLOAT4 MatAmbinent;
	XMFLOAT4 MatDiffuse;
	XMFLOAT4 MatSpecular;
};
//頂点データ
struct VertexM
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};
struct DirectVertexM
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};
struct ModelDate
{
	std::vector<VertexM>vertex;
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
	Vector3 ancAngle;
	Vector3 scale;//サイズ
	OBJMat objmat;
};
class Model
{
public:
	Model(ID3D12Device* device, ID3D12GraphicsCommandList*commandList);
	~Model();
	void Init();
	void CreateBuffModel();
	HRESULT InitModelPipeLine();
	void LoadTex(const wchar_t* filename);
	void CreateNormal();
	void CreateModel(std::vector<std::string>&outMatList, const char* filepath, const wchar_t * filename, int num);
	bool CreateMesh(std::vector<std::string>&outMatList, const char* filepath, const wchar_t* filename);
	bool LoadModel(const wchar_t* filename, int num);
	bool MaterialLoad(std::vector<std::string>fileList, std::string filePath);
	void DrawModel(int num, const Vector3& pos, const Vector3& angle,const Vector3& size);
	void SetMaterial(OBJMat*mat);
	void SetModelAncPoint(int num,Vector3 ancPoint,Vector3 ancAngle);//モデルの原点移動
	void SetSize(const int num, const Vector3 size);//モデルのサイズ

	void Split(char splitChar, char* buffer, std::vector<std::string>&out);//切り取り？
	void Replace(char searchChar, char replaceChar, char*buffer);//取り換え　置き換え
	void ParseVKeyWordTag(std::vector<Vector3>&outVertices, char* buff);
	void ParseFKeyWordTag(std::vector<VertexM>&outObjVert, std::map<std::string, int>&indexList, std::string currentMat, std::vector<Vector3>&vertices, std::vector<Vector3>& textures, std::vector<Vector3>&normals, char*buffer);//インプットレイアウト作成
	void ParseShashKeyWordTag(int* list, char* buffer);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;
	ComPtr<ID3D12DescriptorHeap>modelDescHeap;

	std::unordered_map<const wchar_t*,ComPtr<ID3D12Resource>>modelTexList;//テクスチャリスト(ComPtrに変更)FixMe まだ処理が間違っているかも
	std::unordered_map<const wchar_t*, int>countList;//テクスチャの名前からINTで検索する
	//obj
	std::map<std::string, std::vector<UINT>>mIndices;
	std::map<std::string, OBJMat>mMats;
	std::map<std::string, ID3D12Resource*>mTex;
	ComPtr<ID3D12Resource>spriteTexBuff;
	
	//シェーダファイルの読み込みとコンパイル
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	ComPtr<ID3DBlob>rootSigBlob;
	HRESULT result;
	ModelBuffDate*constMap;//定数バッファにデータを転送
	
	ModelDate modelData;
	std::unordered_map<int, ModelDate>modelDataList;//スプライトのデータリスト
	ComPtr<ID3D12PipelineState>modelPipeLineState;//パイプラインステート
	ComPtr<ID3D12RootSignature>modelRootSignature;//ルートシグネスチャ
	Matrix4 matProjectionM;//射影変換行列
	int count;//テクスチャ番号
};
