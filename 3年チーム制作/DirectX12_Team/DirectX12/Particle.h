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
struct ConstBuffData
{
	Matrix4 mat;
	Matrix4 matBill;
};
struct VertexP
{
	Vector3 pos;//�|�W�V����
	Vector4 color;
	float scale =1.0f;//�T�C�Y
};
struct DirectVertexP
{
	XMFLOAT3 pos;
};
struct ParticleData
{
	std::vector<VertexP>vertex;
	//GPU���\�[�X�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//Matrix4 matWorld;//���[���h�s��
	UINT texNum;//�e�N�X�`���ԍ�
	Vector3 ancPoint3D;
};
//�p�[�e�B�N���ꗱ�̍\����
struct ParticleStateus
{
	Vector3 pos;//�|�W�V����
	Vector3 vel;//���x
	Vector3 acc;//�����x
	Vector4 color;
	float scale = 1.0f;//�T�C�Y
	//�����X�P�[��
	float sScale = 1.0f;
	//�ŏI�X�P�[��
	float eScale = 0.0f;
	int frame = 0;//���݃t���[��
	int numFrame = 0;//�I���t���[��
};

class Particle
{
public:
	

	Particle(ID3D12Device* device, ID3D12GraphicsCommandList*commandList);
	~Particle();
	void Init();
	void Update();
	void CreateBuff();
	void CreatePlane();//�p�[�e�B�N���p�̃|���S��
	HRESULT InitGraphicPipeLine();
	void LoadTex(const wchar_t* filename);
	void CreateParticleData(const wchar_t* filename,const int& num);
	void DrawParticleBill(const int num);//�S�����r���{�[�h
	void DrawParticleBillY(const int& num, const Vector3& pos);//Y���r���{�[�h

	//���X�g�֒ǉ�
	void Add(int life, Vector3 pos, Vector3 vel, Vector3 acc,float sScale,float eScale,Vector4 color);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;
	ConstBuffData*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	std::unordered_map<const wchar_t*, ComPtr<ID3D12Resource>>texList;//�e�N�X�`�����X�g FixMe
	std::unordered_map<const wchar_t*, int>countList;//�e�N�X�`���̖��O����INT�Ō�������
	ParticleData dataP;
	ComPtr<ID3D12DescriptorHeap>DescHeap;
	std::unordered_map<int, ParticleData>dataListP;//�X�v���C�g�̃f�[�^���X�g
	ComPtr<ID3D12PipelineState>PipeLineStateP;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature>RootSignatureP;//���[�g�V�O�l�X�`��
	Matrix4 matProjectionP;//�ˉe�ϊ��s��
	//�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>gsBlob;//�W�I���g���p
	ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob>rootSigBlob;
	int count;//�e�N�X�`���ԍ�
	HRESULT result;
	//�p�[�e�B�N�����X�g
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
