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
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//�E�B���h�E
#include"Window.h"

//���w���C�u����
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
	CUBE,//��
	PLANE,//��
	SPHERE,//��
	NONE,
};

struct ConstBuffDate
{
	Vector4 color;
	Matrix4 mat;
};
//2D�p
struct VertexUV
{
	Vector3 pos;
	Vector2 uv;
};
//3D�p
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

//2D�p
struct SpriteData
{
	//GPU���\�[�X�̐���
	ComPtr<ID3D12Resource> vertBuff;
    //�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Matrix4 matWorld;//���[���h�s��
	Vector4 color;
	UINT texNum;//�e�N�X�`���ԍ�
	Vector2 texSize;//�e�N�X�`���̃T�C�Y
	Vector2 ancPoint;//�A���J�[�|�C���g
	Vector4 texUV;//�e�N�X�`���̐؂���

};
//3D�v
struct SpriteData3D
{
	std::vector<Vertex>vertex;
	std::vector<unsigned short>index;
	//GPU���\�[�X�̐���
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource>indexBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	Matrix4 matWorld;//���[���h�s��
	Vector4 color;
	UINT texNum;//�e�N�X�`���ԍ�
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
	void SetAncPoint(int num, Vector2 ancPoint);//�A���J�[�̃Z�b�g
	void SetSize(int num, Vector2 size);
	void SetUV(int num, float left, float top, float right, float botom, bool inversionX = false, bool inversionY = false);
	void SetColor(int num, Vector4 col);
	void Draw2D(int num,const Vector3& pos,float rotare = 0.0f);
	void Draw3D(int num, const Vector3& pos,const Vector3& angle);
	void Draw3DBill(int num, const Vector3& pos, float rotate = 0.0f);//�S�����r���{�[�h
	void Draw3DBillY(int num, const Vector3& pos, float rotate = 0.0f);//Y���r���{�[�h
	//�����蔻��m�F�p
	void DebugBox(int num,const Vector3& pos, const Vector3& angle);
	void DebugSphere(int num,const Vector3& pos);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;

	SpriteData data;
	std::unordered_map<int ,SpriteData>dataList;//�X�v���C�g�̃f�[�^���X�g
	std::unordered_map<const wchar_t*, ComPtr<ID3D12Resource>>texList;//�e�N�X�`�����X�g FixMe
	std::unordered_map<const wchar_t*, int>countList;//�e�N�X�`���̖��O����INT�Ō�������
	std::vector<VertexUV> vertex;
	ComPtr<ID3D12DescriptorHeap>spriteDescHeap;
	ComPtr<ID3D12PipelineState>spritePipeLineState;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature>spriteRootSignature;//���[�g�V�O�l�X�`��
	ConstBuffDate*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	//�RD�p
	SpriteData3D data3d;
	std::unordered_map<int, SpriteData3D>dataList3d;//�X�v���C�g�̃f�[�^���X�g
	ComPtr<ID3D12PipelineState>PipeLineState3d;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature>RootSignature3d;//���[�g�V�O�l�X�`��
	Matrix4 matProjection3d;//�ˉe�ϊ��s��
	
   //�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob>rootSigBlob;

	Matrix4 matProjection;//�ˉe�ϊ��s��

	int count;//�e�N�X�`���ԍ�
	

	HRESULT result;
	
};
