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
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"

//�v�Z�p
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

//�}�e���A���p
struct OBJMat
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	std::string TexKeyWord;
	std::string TextureName;
};
//�萔�o�b�t�@
struct ModelBuffDate
{
	Vector4 color;
	Matrix4 mat;
	XMFLOAT4 MatAmbinent;
	XMFLOAT4 MatDiffuse;
	XMFLOAT4 MatSpecular;
};
//���_�f�[�^
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
	Vector3 ancAngle;
	Vector3 scale;//�T�C�Y
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
	void SetModelAncPoint(int num,Vector3 ancPoint,Vector3 ancAngle);//���f���̌��_�ړ�
	void SetSize(const int num, const Vector3 size);//���f���̃T�C�Y

	void Split(char splitChar, char* buffer, std::vector<std::string>&out);//�؂���H
	void Replace(char searchChar, char replaceChar, char*buffer);//��芷���@�u������
	void ParseVKeyWordTag(std::vector<Vector3>&outVertices, char* buff);
	void ParseFKeyWordTag(std::vector<VertexM>&outObjVert, std::map<std::string, int>&indexList, std::string currentMat, std::vector<Vector3>&vertices, std::vector<Vector3>& textures, std::vector<Vector3>&normals, char*buffer);//�C���v�b�g���C�A�E�g�쐬
	void ParseShashKeyWordTag(int* list, char* buffer);
private:
	ComPtr<ID3D12Device>dev;
	ComPtr<ID3D12GraphicsCommandList>cmdList;
	ComPtr<ID3D12DescriptorHeap>modelDescHeap;

	std::unordered_map<const wchar_t*,ComPtr<ID3D12Resource>>modelTexList;//�e�N�X�`�����X�g(ComPtr�ɕύX)FixMe �܂��������Ԉ���Ă��邩��
	std::unordered_map<const wchar_t*, int>countList;//�e�N�X�`���̖��O����INT�Ō�������
	//obj
	std::map<std::string, std::vector<UINT>>mIndices;
	std::map<std::string, OBJMat>mMats;
	std::map<std::string, ID3D12Resource*>mTex;
	ComPtr<ID3D12Resource>spriteTexBuff;
	
	//�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob>rootSigBlob;
	HRESULT result;
	ModelBuffDate*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	
	ModelDate modelData;
	std::unordered_map<int, ModelDate>modelDataList;//�X�v���C�g�̃f�[�^���X�g
	ComPtr<ID3D12PipelineState>modelPipeLineState;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature>modelRootSignature;//���[�g�V�O�l�X�`��
	Matrix4 matProjectionM;//�ˉe�ϊ��s��
	int count;//�e�N�X�`���ԍ�
};
