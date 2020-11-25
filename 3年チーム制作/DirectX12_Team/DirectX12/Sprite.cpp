#include "Sprite.h"
#include"WCHAR_char.h"
#include <codecvt> 
#include"Camera.h"
const int spriteSRVCount = 512;//�e�N�X�`���ő喇��
ComPtr<ID3D12Resource>spriteTexBuff[spriteSRVCount];
namespace {
	const int dataSize = 24;
}


Sprite::Sprite(ID3D12Device * device, ID3D12GraphicsCommandList * commandList):
	dev(device),
	cmdList(commandList),
	count(0)
{
	Init();
}

Sprite::~Sprite()
{
	dataList.clear();
	dataList3d.clear();
	texList.clear();
	countList.clear();
}

void Sprite::Init()
{
	CreateVert();//2d
	matProjection = Matrix4::Identity;
	matProjection.m[3][0] = -1.0f;
	matProjection.m[3][1] = 1.0f;
	matProjection.m[0][0] = 2.0f /Window::Window_Width;
	matProjection.m[1][1] = -2.0f / Window::Window_Height;
	
	
	InitGraphicPipeLine();
	InitGraphicPipeLine3D();
}

void Sprite::Update()
{

}

void Sprite::CreateBuff()
{
	//���_�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex[0])*vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.vertBuff));
	VertexUV* vertMap;
	//�o�b�t�@�փf�[�^�𑗐M
	result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	data.vertBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�r���[
	data.vbView.BufferLocation = data.vertBuff->GetGPUVirtualAddress();
	data.vbView.SizeInBytes = sizeof(vertex[0])*vertex.size();
	data.vbView.StrideInBytes = sizeof(vertex[0]);
	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffDate)+0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.constBuff));

	result = data.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color = data.color;
	constMap->mat = matProjection;
	data.constBuff->Unmap(0, nullptr);
}

void Sprite::CreateBuff3D()
{
	//���_�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(data3d.vertex[0])*data3d.vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data3d.vertBuff));
	Vertex* vertMap = nullptr;
	//index�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(data3d.index[0])*data3d.index.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data3d.indexBuff));

	unsigned short* indexMap = nullptr;
	

	//�o�b�t�@�փf�[�^�𑗐M
	result = data3d.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, data3d.vertex.data(), sizeof(data3d.vertex[0])* data3d.vertex.size());
	data3d.vertBuff->Unmap(0, nullptr);
	//�o�b�t�@�փf�[�^�𑗐M
	result = data3d.indexBuff->Map(0, nullptr, (void**)&indexMap);
	memcpy(indexMap, data3d.index.data(), sizeof(data3d.index[0])* data3d.index.size());
	data3d.indexBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[
	data3d.vbView.BufferLocation = data3d.vertBuff->GetGPUVirtualAddress();
	data3d.vbView.SizeInBytes = sizeof(data3d.vertex[0])*data3d.vertex.size();
	data3d.vbView.StrideInBytes = sizeof(data3d.vertex[0]);
	
	//index�o�b�t�@�r���[
	data3d.ibView.BufferLocation = data3d.indexBuff->GetGPUVirtualAddress();
	data3d.ibView.Format = DXGI_FORMAT_R16_UINT;
	data3d.ibView.SizeInBytes = sizeof(data3d.index[0])* data3d.index.size();

	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data3d.constBuff));

	XMMATRIX xx = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Window::Window_Width / Window::Window_Height,
		0.1f, 1000.0f
	);
	result = data3d.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color = data3d.color;
	float xxx[4][4]=
	{
		{xx.r[0].m128_f32[0],xx.r[0].m128_f32[1],xx.r[0].m128_f32[2],xx.r[0].m128_f32[3]},
		{xx.r[1].m128_f32[0],xx.r[1].m128_f32[1],xx.r[1].m128_f32[2],xx.r[1].m128_f32[3]},
		{xx.r[2].m128_f32[0],xx.r[2].m128_f32[1],xx.r[2].m128_f32[2],xx.r[2].m128_f32[3]},
		{xx.r[3].m128_f32[0],xx.r[3].m128_f32[1],xx.r[3].m128_f32[2],xx.r[3].m128_f32[3]}
	};

	matProjection3d = Matrix4(xxx);
	constMap->mat = matProjection3d;
	data3d.constBuff->Unmap(0, nullptr);

}

void Sprite::CreateVert()
{
	vertex.clear();
	vertex.resize(4);

	vertex[0].pos.x = 0.0f;//����
	vertex[0].pos.y = 10.0f;
	vertex[0].pos.z = 0.0f;
	vertex[0].uv.x = 0.0f;
	vertex[0].uv.y = 1.0f;

	vertex[1].pos.x = 0.0f;//����
	vertex[1].pos.y = 0.0f;
	vertex[1].pos.z = 0.0f;
	vertex[1].uv.x = 0.0f;
	vertex[1].uv.y = 0.0f;

	vertex[2].pos.x = 10.0f;//�E��
	vertex[2].pos.y = 10.0f;
	vertex[2].pos.z = 0.0f;
	vertex[2].uv.x = 1.0f;
	vertex[2].uv.y = 1.0f;

	vertex[3].pos.x = 10.0f;//�E��
	vertex[3].pos.y = 0.0f;
	vertex[3].pos.z = 0.0f;
	vertex[3].uv.x = 1.0f;
	vertex[3].uv.y = 0.0f;
}

void Sprite::CreateVertCube()
{
	data3d.vertex.clear();
	data3d.vertex.resize(24);
	float size = 10.0f;
	//��O

	//����
	data3d.vertex[0].pos.x = -size;
	data3d.vertex[0].pos.y = -size;
	data3d.vertex[0].pos.z = -size;
	data3d.vertex[0].uv.x = 0.0f;
	data3d.vertex[0].uv.y = 1.0f;

	//����
	data3d.vertex[1].pos.x = -size;
	data3d.vertex[1].pos.y = size;
	data3d.vertex[1].pos.z = -size;
	data3d.vertex[1].uv.x = 0.0f;
	data3d.vertex[1].uv.y = 0.0f;

	//�E��
	data3d.vertex[2].pos.x = size;
	data3d.vertex[2].pos.y = -size;
	data3d.vertex[2].pos.z = -size;
	data3d.vertex[2].uv.x = 1.0f;
	data3d.vertex[2].uv.y = 1.0f;

	//�E��
	data3d.vertex[3].pos.x = size;
	data3d.vertex[3].pos.y = size;
	data3d.vertex[3].pos.z = -size;
	data3d.vertex[3].uv.x = 1.0f;
	data3d.vertex[3].uv.y = 0.0f;

	//���
	//����
	data3d.vertex[4].pos.x = -size;
	data3d.vertex[4].pos.y = -size;
	data3d.vertex[4].pos.z = size;
	data3d.vertex[4].uv.x = 1.0f;
	data3d.vertex[4].uv.y = 0.0f;

	//����
	data3d.vertex[5].pos.x = -size;
	data3d.vertex[5].pos.y = size;
	data3d.vertex[5].pos.z = size;
	data3d.vertex[5].uv.x = 1.0f;
	data3d.vertex[5].uv.y = 1.0f;

	//�E��
	data3d.vertex[6].pos.x = size;
	data3d.vertex[6].pos.y = -size;
	data3d.vertex[6].pos.z = size;
	data3d.vertex[6].uv.x = 0.0f;
	data3d.vertex[6].uv.y = 0.0f;

	//�E��
	data3d.vertex[7].pos.x = size;
	data3d.vertex[7].pos.y = size;
	data3d.vertex[7].pos.z = size;
	data3d.vertex[7].uv.x = 0.0f;
	data3d.vertex[7].uv.y = 1.0f;

	//��
	//����
	data3d.vertex[8].pos.x = -size;
	data3d.vertex[8].pos.y = -size;
	data3d.vertex[8].pos.z = -size;
	data3d.vertex[8].uv.x = 0.0f;
	data3d.vertex[8].uv.y = 1.0f;

	//����
	data3d.vertex[9].pos.x = -size;
	data3d.vertex[9].pos.y = -size;
	data3d.vertex[9].pos.z = size;
	data3d.vertex[9].uv.x = 0.0f;
	data3d.vertex[9].uv.y = 0.0f;

	//�E��
	data3d.vertex[10].pos.x = -size;
	data3d.vertex[10].pos.y = size;
	data3d.vertex[10].pos.z = -size;
	data3d.vertex[10].uv.x = 1.0f;
	data3d.vertex[10].uv.y = 1.0f;

	//�E��
	data3d.vertex[11].pos.x = -size;
	data3d.vertex[11].pos.y = size;
	data3d.vertex[11].pos.z = size;
	data3d.vertex[11].uv.x = 1.0f;
	data3d.vertex[11].uv.y = 0.0f;

	//�E
	//����
	data3d.vertex[12].pos.x = size;
	data3d.vertex[12].pos.y = -size;
	data3d.vertex[12].pos.z = -size;
	data3d.vertex[12].uv.x = 0.0f;
	data3d.vertex[12].uv.y = 1.0f;

	//����
	data3d.vertex[13].pos.x = size;
	data3d.vertex[13].pos.y = size;
	data3d.vertex[13].pos.z = -size;
	data3d.vertex[13].uv.x = 0.0f;
	data3d.vertex[13].uv.y = 0.0f;

	//�E��
	data3d.vertex[14].pos.x = size;
	data3d.vertex[14].pos.y = -size;
	data3d.vertex[14].pos.z = size;
	data3d.vertex[14].uv.x = 1.0f;
	data3d.vertex[14].uv.y = 1.0f;

	//�E��
	data3d.vertex[15].pos.x = size;
	data3d.vertex[15].pos.y = size;
	data3d.vertex[15].pos.z = size;
	data3d.vertex[15].uv.x = 1.0f;
	data3d.vertex[15].uv.y = 0.0f;

	//��
	//����
	data3d.vertex[16].pos.x = -size;
	data3d.vertex[16].pos.y = size;
	data3d.vertex[16].pos.z = -size;
	data3d.vertex[16].uv.x = 0.0f;
	data3d.vertex[16].uv.y = 1.0f;

	//����
	data3d.vertex[17].pos.x = -size;
	data3d.vertex[17].pos.y = size;
	data3d.vertex[17].pos.z = size;
	data3d.vertex[17].uv.x = 0.0f;
	data3d.vertex[17].uv.y = 0.0f;

	//�E��
	data3d.vertex[18].pos.x = size;
	data3d.vertex[18].pos.y = size;
	data3d.vertex[18].pos.z = -size;
	data3d.vertex[18].uv.x = 1.0f;
	data3d.vertex[18].uv.y = 1.0f;

	//�E��
	data3d.vertex[19].pos.x = size;
	data3d.vertex[19].pos.y = size;
	data3d.vertex[19].pos.z = size;
	data3d.vertex[19].uv.x = 1.0f;
	data3d.vertex[19].uv.y = 0.0f;

	//��
	//����
	data3d.vertex[20].pos.x = -size;
	data3d.vertex[20].pos.y = -size;
	data3d.vertex[20].pos.z = -size;
	data3d.vertex[20].uv.x = 0.0f;
	data3d.vertex[20].uv.y = 1.0f;

	//����
	data3d.vertex[21].pos.x = size;
	data3d.vertex[21].pos.y = -size;
	data3d.vertex[21].pos.z = -size;
	data3d.vertex[21].uv.x = 0.0f;
	data3d.vertex[21].uv.y = 0.0f;

	//�E��
	data3d.vertex[22].pos.x = -size;
	data3d.vertex[22].pos.y = -size;
	data3d.vertex[22].pos.z = size;
	data3d.vertex[22].uv.x = 1.0f;
	data3d.vertex[22].uv.y = 1.0f;

	//�E��
	data3d.vertex[23].pos.x = size;
	data3d.vertex[23].pos.y = -size;
	data3d.vertex[23].pos.z = size;
	data3d.vertex[23].uv.x = 1.0f;
	data3d.vertex[23].uv.y = 0.0f;


	data3d.index.clear();
	data3d.index.resize(36);
	//�O
	data3d.index[0] = 0;
	data3d.index[1] = 1;
	data3d.index[2] = 2;
	data3d.index[3] = 2;
	data3d.index[4] = 1;
	data3d.index[5] = 3;

	//���
	data3d.index[6] = 4;
	data3d.index[7] = 5;
	data3d.index[8] = 6;
	data3d.index[9] = 5;
	data3d.index[10] = 7;
	data3d.index[11] = 6;

	//��
	data3d.index[12] = 8;
	data3d.index[13] = 9;
	data3d.index[14] = 10;
	data3d.index[15] = 9;
	data3d.index[16] = 11;
	data3d.index[17] = 10;

	//�E
	data3d.index[18] = 12;
	data3d.index[19] = 13;
	data3d.index[20] = 14;
	data3d.index[21] = 13;
	data3d.index[22] = 15;
	data3d.index[23] = 14;

	//��
	data3d.index[24] = 16;
	data3d.index[25] = 17;
	data3d.index[26] = 18;
	data3d.index[27] = 17;
	data3d.index[28] = 19;
	data3d.index[29] = 18;

	//��
	data3d.index[30] = 20;
	data3d.index[31] = 21;
	data3d.index[32] = 22;
	data3d.index[33] = 21;
	data3d.index[34] = 23;
	data3d.index[35] = 22;

	
}

void Sprite::CreateVertSphere()
{
	float x, y, z;
	x = 100.0f;
	y = 100.0f;
	z = 10.0f;
	data3d.vertex.clear();
	data3d.vertex.resize(36);
	float R = 0.5f;
	for (int i = 0; i < data3d.vertex.size(); i++)
	{
		x = R * sin((XM_PI / data3d.vertex.size())*i);
		x = x * Window::Window_Height / Window::Window_Width;
		y = R * cos((XM_PI / data3d.vertex.size())*i);
		data3d.vertex[i].pos.x = x;
		data3d.vertex[i].pos.y = y;
		data3d.vertex[i].pos.z = z;
	}

	data3d.vertex[data3d.vertex.size()-1].pos.x = 0;
	data3d.vertex[data3d.vertex.size()-1].pos.y = 0;
	data3d.vertex[data3d.vertex.size()-1].pos.z = 0;


	//�C���f�b�N�X
	data3d.index.clear();
	int count = data3d.vertex.size() * 3;
	data3d.index.resize(count);
	for (int i = 0;i< data3d.vertex.size();i++)
	{
		data3d.index[i * 3] = data3d.vertex.size()- 1;
		data3d.index[(i * 3) + 1] = i + 0;
		data3d.index[(i * 3) + 2] = i + 1;
	}
	data3d.index[(data3d.vertex.size() * 3) - 1] = 0;
}

void Sprite::CreateVertPlane()
{
	data3d.vertex.clear();
	data3d.vertex.resize(4);
	//��O

	//����
	data3d.vertex[0].pos.x = -500.0f;
	data3d.vertex[0].pos.y = -500.0f;
	data3d.vertex[0].pos.z = 0.0f;
	data3d.vertex[0].uv.x = 0.0f;
	data3d.vertex[0].uv.y = 1.0f;

	//����
	data3d.vertex[1].pos.x = -500.0f;
	data3d.vertex[1].pos.y = 500.0f;
	data3d.vertex[1].pos.z = 0.0f;
	data3d.vertex[1].uv.x = 0.0f;
	data3d.vertex[1].uv.y = 0.0f;

	//�E��
	data3d.vertex[2].pos.x = 500.0f;
	data3d.vertex[2].pos.y = -500.0f;
	data3d.vertex[2].pos.z = 0.0f;
	data3d.vertex[2].uv.x = 1.0f;
	data3d.vertex[2].uv.y = 1.0f;

	//�E��
	data3d.vertex[3].pos.x = 500.0f;
	data3d.vertex[3].pos.y = 500.0f;
	data3d.vertex[3].pos.z = 0.0f;
	data3d.vertex[3].uv.x = 1.0f;
	data3d.vertex[3].uv.y = 0.0f;

	data3d.index.clear();
	data3d.index.resize(6);
	//�O
	data3d.index[0] = 0;
	data3d.index[1] = 1;
	data3d.index[2] = 2;
	data3d.index[3] = 2;
	data3d.index[4] = 1;
	data3d.index[5] = 3;
}

void Sprite::CreateNormal()
{
	std::vector<DirectVertex>v;
	v.clear();
	v.resize(data3d.vertex.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i].pos.x = data3d.vertex[i].pos.x;
		v[i].pos.y = data3d.vertex[i].pos.y;
		v[i].pos.z = data3d.vertex[i].pos.z;

		v[i].normal.x = data3d.vertex[i].normal.x;
		v[i].normal.y = data3d.vertex[i].normal.y;
		v[i].normal.z = data3d.vertex[i].normal.z;

		v[i].uv.x = data3d.vertex[i].uv.x;
		v[i].uv.y = data3d.vertex[i].uv.y;
	}

	//�@���v�Z
	for (int i = 0; i < data3d.index.size() / 3; i++)
	{
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = data3d.index[i * 3 + 0];
		unsigned short index1 = data3d.index[i * 3 + 1];
		unsigned short index2 = data3d.index[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&v[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&v[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&v[index2].pos);

		//�x�N�g���̌��Z
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&v[index0].normal, normal);
		XMStoreFloat3(&v[index1].normal, normal);
		XMStoreFloat3(&v[index2].normal, normal);
	}
	for (size_t i = 0; i < v.size(); ++i)
	{
		data3d.vertex[i].normal.x = v[i].normal.x;
		data3d.vertex[i].normal.y = v[i].normal.y;
		data3d.vertex[i].normal.z = v[i].normal.z;
	}
	v.clear();
}

HRESULT Sprite::InitGraphicPipeLine()
{
	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	//VS
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob
	);
	//PS
	result = D3DCompileFromFile(
		L"SpritePixelShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&psBlob, &errorBlob
	);
	//�V�F�[�_�[�̃G���[���e�̊m�F
	//errorBlob����G���[���󂯎��string�ɂ���
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	//���_���C�A�E�g
	//���_�V�F�[�_�[�ɓn���悤�̒��_�f�[�^�𐮂���
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },
		 {
			 "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			  D3D12_APPEND_ALIGNED_ELEMENT,
			  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },

	};

	//�O���t�B�b�N�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeLine{};
	gpipeLine.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeLine.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�ƃ��X�^���C�Y�X�e�[�g�ݒ�
	gpipeLine.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//	�W��
	gpipeLine.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeLine.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�w�ʃJ�����O���Ȃ�

	//�[�x�e�X�g
	gpipeLine.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W��

	//���ʐݒ�
	blenddesc.BlendEnable = true;//�u�����h�L��
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�l�P�O�O��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�l�O��
	//������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//�u�����h�X�e�[�g�ɐݒ�
	gpipeLine.BlendState.RenderTarget[0] = blenddesc;
	//�[�x�o�b�t�@�t�H�[�}�b�g
	gpipeLine.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//�f�v�X�̏������݋֎~
	//gpipeLine.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//���_���C�A�E�g�̐ݒ�
	gpipeLine.InputLayout.pInputElementDescs = inputLayout;
	gpipeLine.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`����O�p�`��
	gpipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��ݒ�
	gpipeLine.NumRenderTargets = 1;//�`��Ώۈ��
	gpipeLine.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeLine.SampleDesc.Count = 1;
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//�T���v���[�ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//���[�g�V�O�l�X�`���̐���
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//�o�[�W������������
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
//���[�g�V�O�l�X�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));

	//�O���t�B�b�N�p�C�v���C���X�e�[�g�̐����ɃZ�b�g
	gpipeLine.pRootSignature = spriteRootSignature.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeLine, IID_PPV_ARGS(&spritePipeLineState));
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));

	return S_OK;

}

HRESULT Sprite::InitGraphicPipeLine3D()
{
	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	//VS
	result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob
	);
	//PS
	result = D3DCompileFromFile(
		L"BasicPixelShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&psBlob, &errorBlob
	);
	
	//�V�F�[�_�[�̃G���[���e�̊m�F
	//errorBlob����G���[���󂯎��string�ɂ���
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	//���_���C�A�E�g
	//���_�V�F�[�_�[�ɓn���悤�̒��_�f�[�^�𐮂���
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },
		 {
			 //�@���x�N�g��
			 "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			 D3D12_APPEND_ALIGNED_ELEMENT,
			 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			 D3D12_APPEND_ALIGNED_ELEMENT,
			 D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�O���t�B�b�N�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeLine{};
	gpipeLine.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeLine.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�ƃ��X�^���C�Y�X�e�[�g�ݒ�
	gpipeLine.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//	�W��
	gpipeLine.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeLine.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//�w�ʃJ�����O���Ȃ�
	//�[�x�e�X�g
	gpipeLine.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//gpipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��
	gpipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W��

	//���ʐݒ�
	blenddesc.BlendEnable = true;//�u�����h�L��
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�l�P�O�O��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�l�O��
	//������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//�u�����h�X�e�[�g�ɐݒ�
	gpipeLine.BlendState.RenderTarget[0] = blenddesc;
	//�[�x�o�b�t�@�t�H�[�}�b�g
	gpipeLine.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//���_���C�A�E�g�̐ݒ�
	gpipeLine.InputLayout.pInputElementDescs = inputLayout;
	gpipeLine.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`����O�p�`��
	gpipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��ݒ�
	gpipeLine.NumRenderTargets = 1;//�`��Ώۈ��
	gpipeLine.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeLine.SampleDesc.Count = 1;
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//�T���v���[�ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//���[�g�V�O�l�X�`���̐���
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//�o�[�W������������
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	//���[�g�V�O�l�X�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature3d));

	//�O���t�B�b�N�p�C�v���C���X�e�[�g�̐����ɃZ�b�g
	gpipeLine.pRootSignature = RootSignature3d.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeLine, IID_PPV_ARGS(&PipeLineState3d));
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));

	return S_OK;


}

void Sprite::LoadTex(const wchar_t * filename)
{
	auto itr = texList.find(filename);
	if (itr != texList.end())//���ɓo�^�ςȂ�
	{
		return;
	}
	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	const Image* img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o
	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	//�e�N�X�`���p�o�b�t�@�̍쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���w��p
		nullptr,
		IID_PPV_ARGS(&spriteTexBuff[count])
	);
	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = spriteTexBuff[count]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);
	//�V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//�Q���e�N�X�`
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(
		spriteTexBuff[count].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteDescHeap->GetCPUDescriptorHandleForHeapStart(),
			count, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	texList.emplace(filename, spriteTexBuff[count].Get());
	countList.emplace(filename, count);
	count++;
}

void Sprite::CreateSpliteDate(const wchar_t * filename, int num)
{
	data.texNum = countList[filename];
	data.color = Vector4(1, 1, 1, 1);
	//���_�f�[�^���摜�T�C�Y�ɏ�������
	data.texSize.x = (float)texList[filename]->GetDesc().Width;
	data.texSize.y = (float)texList[filename]->GetDesc().Height;

	CreateVert();
	vertex[0].pos = Vector3(0.0f, data.texSize.y, 0.0f);//����
	vertex[1].pos = Vector3(0.0f, 0.0f, 0.0f);//����
	vertex[2].pos = Vector3(data.texSize.x, data.texSize.y, 0.0f);//�E��
	vertex[3].pos = Vector3(data.texSize.x, 0.0f, 0.0f);//�E��

	data.ancPoint = Vector2(0.0f, 0.0f);
	data.texUV = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	CreateBuff();
	dataList.emplace(num, data);

}

void Sprite::CreateSpliteDate3D(const wchar_t * filename, int num,VertType type)
{
	switch (type)
	{
	case VertType::CUBE:
		CreateVertCube();
		break;
	case VertType::PLANE:
		CreateVertPlane();
		break;
	case VertType::SPHERE:
		CreateVertSphere();
		break;
	default:
		break;
	}
	CreateNormal();
	data3d.texNum = countList[filename];
	data3d.color = Vector4(1, 1, 1, 1);
	CreateBuff3D();
	dataList3d.emplace(num, data3d);
}
void Sprite::SetAncPoint(int num, Vector2 ancPoint)
{
	auto d = dataList[num];
	dataList.erase(num);
	d.ancPoint = ancPoint;
	dataList.emplace(num, d);
}

void Sprite::SetSize(int num, Vector2 size)
{
	auto d = dataList[num];
	dataList.erase(num);
	d.texSize = size;
	dataList.emplace(num, d);
}


void Sprite::SetUV(int num, float left, float top, float right, float bottom, bool inversionX, bool inversionY)
{
	auto d = dataList[num];
	dataList.erase(num);
	float Width = d.texSize.x;
	float Height = d.texSize.y;
	float Left = (0.0f, -d.ancPoint.x)*Width;
	float Right = (1.0f - d.ancPoint.x)*Width;
	float Top = (0.0f - d.ancPoint.y)*Height;
	float Bottom = (1.0f - d.ancPoint.y)*Height;
	//���]
	if (inversionX)
	{
		Left = -Left;
		Right = -Right;
	}

	if (inversionY)
	{
		Top = -Top;
		Bottom = -Bottom;
	}

	vertex[0].pos = Vector3(Left, Bottom, 0.0f);
	vertex[1].pos = Vector3(Left, Top, 0.0f);
	vertex[2].pos = Vector3(Right, Bottom, 0.0f);
	vertex[3].pos = Vector3(Right, Top, 0.0f);

	d.texUV = Vector4(left, top, right, bottom);
	vertex[0].uv = Vector2(left, bottom);
	vertex[1].uv = Vector2(left, top);
	vertex[2].uv = Vector2(right, bottom);
	vertex[3].uv = Vector2(right, top);

	VertexUV* vertMap;
	//�o�b�t�@�փf�[�^�𑗐M
	result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	data.vertBuff->Unmap(0, nullptr);
	dataList.emplace(num, d);
}

void Sprite::SetColor(int num, Vector4 col)
{
	auto d = dataList[num];
	dataList.erase(num);
	d.color = col;
	dataList.emplace(num, d);
}

void Sprite::Draw2D(int num, const Vector3& pos, float rotare)
{
	auto d = dataList[num];
	d.matWorld =Matrix4::Identity;
	d.matWorld *= Matrix4::RotateZ(rotare);
	d.matWorld *= Matrix4::createTranslation(pos);
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = d.matWorld*matProjection;
	constMap->color = d.color;
	d.constBuff->Unmap(0, nullptr);
	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(spritePipeLineState.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);

	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawInstanced(vertex.size(), 1, 0, 0);

}

void Sprite::Draw3D(int num, const Vector3 & pos,const Vector3& angle)
{
	auto d = dataList3d[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
	d.matWorld = Matrix4::Identity;
	d.matWorld *= Matrix4::RotateZ(angle.z);
	d.matWorld *= Matrix4::RotateY(angle.y);
	d.matWorld *= Matrix4::RotateX(angle.x);
	d.matWorld *= Matrix4::createTranslation(Vector3(pos.x,pos.y,pos.z));
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = d.color;
	constMap->mat = d.matWorld*Camera::matView *matProjection3d;
	//constMap->mat = Camera::matView * matProjection3d;//�W�I���g���p
	d.constBuff->Unmap(0, nullptr);

	
	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineState3d.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignature3d.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//�W�I���g���p
	//cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	cmdList->IASetIndexBuffer(&d.ibView);
	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(d.index.size(), 1, 0, 0,0);
}

void Sprite::Draw3DBill(int num, const Vector3 & pos, float rotate)
{
	Matrix4 matBill = Matrix4::Identity;
	auto d = dataList3d[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
	d.matWorld *= Matrix4::createScale(1, 1, 1);
	Vector3 zAxis = Vector3::normalize(Camera::target - Camera::eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(Camera::up, zAxis));
	Vector3 yAxis = Vector3::normalize(Vector3::cross(zAxis, xAxis));
	float temp[4][4] =
	{
		{xAxis.x,xAxis.y,xAxis.z,0.0f},
		{yAxis.x,yAxis.y,yAxis.z,0.0f},
		{zAxis.x,zAxis.y,zAxis.z,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	};
	matBill = Matrix4(temp);
	d.matWorld *= matBill;
	d.matWorld *= Matrix4::RotateZ(rotate);
	d.matWorld *= Matrix4::createTranslation(Vector3(pos.x, pos.y, pos.z));
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = d.color;
	constMap->mat = d.matWorld*Camera::matView *matProjection3d;
	//constMap->mat = Camera::matView * matProjection3d;//�W�I���g���p
	d.constBuff->Unmap(0, nullptr);


	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineState3d.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignature3d.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	cmdList->IASetIndexBuffer(&d.ibView);
	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(d.index.size(), 1, 0, 0, 0);
	
}

void Sprite::Draw3DBillY(int num, const Vector3 & pos, float rotate)
{
	Matrix4 matBill = Matrix4::Identity;
	auto d = dataList3d[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
	d.matWorld = Matrix4::Identity;
	d.matWorld *= Matrix4::createScale(1, 1, 1);

	Vector3 zAxis = Vector3::normalize(Camera::target - Camera::eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(Camera::up, zAxis));
	Vector3 yAxis = Vector3::normalize(Camera::up);
	zAxis = Vector3::cross(xAxis, yAxis);
	float temp[4][4] =
	{
		{xAxis.x,xAxis.y,xAxis.z,0.0f},
		{yAxis.x,yAxis.y,yAxis.z,0.0f},
		{zAxis.x,zAxis.y,zAxis.z,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	};
	matBill = Matrix4(temp);
	d.matWorld *= matBill;
	d.matWorld *= Matrix4::RotateZ(rotate);
	d.matWorld *= Matrix4::createTranslation(Vector3(pos.x, pos.y, pos.z));
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = d.color;
	constMap->mat = d.matWorld*Camera::matView *matProjection3d;
	d.constBuff->Unmap(0, nullptr);


	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineState3d.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignature3d.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	cmdList->IASetIndexBuffer(&d.ibView);
	
	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(d.index.size(), 1, 0, 0, 0);
	
}

void Sprite::DebugBox(int num,const Vector3& pos,const Vector3& angle)
{
	auto d = dataList3d[num];
	d.matWorld = Matrix4::Identity;
	d.matWorld *= Matrix4::RotateZ(angle.z);
	d.matWorld *= Matrix4::RotateY(angle.y);
	d.matWorld *= Matrix4::RotateX(angle.x);
	d.matWorld *= Matrix4::createTranslation(Vector3(pos.x, pos.y, pos.z));
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = d.color;
	constMap->mat = d.matWorld*Camera::matView *matProjection3d;
	//constMap->mat = Camera::matView * matProjection3d;//�W�I���g���p
	d.constBuff->Unmap(0, nullptr);


	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineState3d.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignature3d.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	cmdList->IASetIndexBuffer(&d.ibView);
	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(d.index.size(), 1, 0, 0, 0);
	//cmdList->DrawInstanced(d.vertex.size(), 1, 0, 0);//���_�̂݃W�I���g���p
}

void Sprite::DebugSphere(int num ,const Vector3& pos)
{
}
