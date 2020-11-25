#include "Model.h"
#include"WCHAR_char.h"
#include <codecvt> 
#include"Camera.h"
#include"Window.h"
const int modelSRVCount = 512;//�e�N�X�`���ő喇��
ComPtr<ID3D12Resource>TexBuffM[modelSRVCount];
namespace {
	const int dataSize = 24;
}

Model::Model(ID3D12Device * device, ID3D12GraphicsCommandList * commandList)
	:dev(device),cmdList(commandList),count(0)
{
	Init();
}

Model::~Model()
{
	modelDataList.clear();
	modelTexList.clear();
	countList.clear();
}

void Model::Init()
{
	matProjectionM = Matrix4::Identity;
	matProjectionM.m[3][0] = -1.0f;
	matProjectionM.m[3][1] = 1.0f;
	matProjectionM.m[0][0] = 2.0f / Window::Window_Width;
	matProjectionM.m[1][1] = -2.0f / Window::Window_Height;

	InitModelPipeLine();
}

void Model::CreateBuffModel()
{
	//���_�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(modelData.vertex[0])*modelData.vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelData.vertBuff));
	VertexM* vertMap = nullptr;

	//index�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(modelData.index[0])*modelData.index.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelData.indexBuff));

	unsigned short* indexMap = nullptr;

	//�o�b�t�@�փf�[�^�𑗐M
	result = modelData.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, modelData.vertex.data(), sizeof(modelData.vertex[0])* modelData.vertex.size());
	modelData.vertBuff->Unmap(0, nullptr);

	
	//�o�b�t�@�փf�[�^�𑗐M
	result = modelData.indexBuff->Map(0, nullptr, (void**)&indexMap);
	memcpy(indexMap, modelData.index.data(), sizeof(modelData.index[0])* modelData.index.size());
	modelData.indexBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[
	modelData.vbView.BufferLocation = modelData.vertBuff->GetGPUVirtualAddress();
	modelData.vbView.SizeInBytes = sizeof(modelData.vertex[0])*modelData.vertex.size();
	modelData.vbView.StrideInBytes = sizeof(modelData.vertex[0]);
	//index�o�b�t�@�r���[
	modelData.ibView.BufferLocation = modelData.indexBuff->GetGPUVirtualAddress();
	modelData.ibView.Format = DXGI_FORMAT_R16_UINT;
	modelData.ibView.SizeInBytes = sizeof(modelData.index[0])* modelData.index.size();

	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ModelBuffDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelData.constBuff));

	XMMATRIX xx = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Window::Window_Width / Window::Window_Height,
		0.1f, 1000.0f
	);
	result = modelData.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color = modelData.color;
	float xxx[4][4] =
	{
		{xx.r[0].m128_f32[0],xx.r[0].m128_f32[1],xx.r[0].m128_f32[2],xx.r[0].m128_f32[3]},
		{xx.r[1].m128_f32[0],xx.r[1].m128_f32[1],xx.r[1].m128_f32[2],xx.r[1].m128_f32[3]},
		{xx.r[2].m128_f32[0],xx.r[2].m128_f32[1],xx.r[2].m128_f32[2],xx.r[2].m128_f32[3]},
		{xx.r[3].m128_f32[0],xx.r[3].m128_f32[1],xx.r[3].m128_f32[2],xx.r[3].m128_f32[3]}
	};

	matProjectionM = Matrix4(xxx);
	constMap->mat = matProjectionM;
	modelData.constBuff->Unmap(0, nullptr);
}

HRESULT Model::InitModelPipeLine()
{
	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	//VS
	result = D3DCompileFromFile(
		L"ModelVertexShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob
	);
	//PS
	result = D3DCompileFromFile(
		L"ModelPixelShader.hlsl",//�V�F�[�_�[�t�@�C����
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
	//gpipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;//�W�I���g���p
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
		IID_PPV_ARGS(&modelRootSignature));

	//�O���t�B�b�N�p�C�v���C���X�e�[�g�̐����ɃZ�b�g
	gpipeLine.pRootSignature = modelRootSignature.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeLine, IID_PPV_ARGS(&modelPipeLineState));
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = modelSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&modelDescHeap));

	return S_OK;

}

void Model::LoadTex(const wchar_t * filename)
{
	auto itr = modelTexList.find(filename);
	if (itr != modelTexList.end())//���ɓo�^�ςȂ�
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
		IID_PPV_ARGS(&TexBuffM[count])
	);
	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = TexBuffM[count]->WriteToSubresource(
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
		TexBuffM[count].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(modelDescHeap->GetCPUDescriptorHandleForHeapStart(),
			count, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	modelTexList.emplace(filename, TexBuffM[count].Get());
	countList.emplace(filename, count);
	count++;
}

void Model::CreateNormal()
{
	std::vector<DirectVertexM>v;
	v.clear();
	v.resize(modelData.vertex.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i].pos.x = modelData.vertex[i].pos.x;
		v[i].pos.y = modelData.vertex[i].pos.y;
		v[i].pos.z = modelData.vertex[i].pos.z;

		v[i].normal.x = modelData.vertex[i].normal.x;
		v[i].normal.y = modelData.vertex[i].normal.y;
		v[i].normal.z = modelData.vertex[i].normal.z;

		v[i].uv.x = modelData.vertex[i].uv.x;
		v[i].uv.y = modelData.vertex[i].uv.y;
	}

	//�@���v�Z
	for (int i = 0; i < modelData.index.size() / 3; i++)
	{
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = modelData.index[i * 3 + 0];
		unsigned short index1 = modelData.index[i * 3 + 1];
		unsigned short index2 = modelData.index[i * 3 + 2];

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
		modelData.vertex[i].normal.x = v[i].normal.x;
		modelData.vertex[i].normal.y = v[i].normal.y;
		modelData.vertex[i].normal.z = v[i].normal.z;
	}
	v.clear();
}

void Model::CreateModel(std::vector<std::string>& outMatList, const char * filepath, const wchar_t * filename, int num)
{
	modelData.texNum = countList[filename];
	modelData.color = Vector4(1, 1, 1, 1);
	modelData.scale = Vector3(1, 1, 1);
	modelData.ancPoint3D = Vector3(0.0f, 0.0f, 0.0f);
	CreateBuffModel();
	modelDataList.emplace(num, modelData);
}

bool Model::CreateMesh(std::vector<std::string>& outMatList, const char * filepath, const wchar_t * filename)
{
	FILE*fp = nullptr;
	std::vector<Vector3>vertices;
	std::vector<Vector3>normals;
	std::vector<Vector3>texes;
	std::map<std::string, int>indexList;
	std::string currentMatName = "";
	char loacal[dataSize + 1] = { 0x00 };

	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		filename,
		-1,
		loacal,
		dataSize + 1,
		NULL,
		NULL
	);


	fopen_s(&fp, loacal, "r");
	if (fp == nullptr)
	{
		return false;
	}

	const int LineBufferLength = 1024;
	char buffer[LineBufferLength];

	while (fgets(buffer, LineBufferLength, fp) != nullptr)
	{
		//�R�����g�͖���
		if (buffer[0] == '#')
		{
			continue;
		}
		char* parsePoint = strchr(buffer, ' ');
		if (parsePoint == nullptr)
		{
			continue;
		}
		Replace('\n', '\0', buffer);
		//���_�֘A
		if (buffer[0] == 'v')
		{
			//���_���W
			if (buffer[1] == ' ')
			{
				ParseVKeyWordTag(vertices, &parsePoint[1]);
				//z���̔��]
				vertices[vertices.size() - 1].z *= -1.0f;
				//vertices[vertices.size() - 1].x *= -1.0f;
			}
			else if (buffer[1] == 't')
			{
				ParseVKeyWordTag(texes, &parsePoint[1]);
				texes[texes.size() - 1].y = (1.0f - texes[texes.size() - 1].y);
			}
			else if (buffer[1] == 'n')//�@��
			{
				ParseVKeyWordTag(normals, &parsePoint[1]);
				//z����]
				normals[normals.size() - 1].z *= -1.0f;
				//normals[normals.size() - 1].x *= -1.0f;
			}

		}
		//��
		else if (buffer[0] == 'f')
		{
			ParseFKeyWordTag(modelData.vertex, indexList, currentMatName, vertices, texes, normals, &parsePoint[1]);
		}
		else if (strstr(buffer, "mtllib") == buffer)
		{
			Replace('\n', '\0', buffer);
			//�}�e���A������ۑ�
			outMatList.push_back(&buffer[strlen("mtllib") + 1]);
		}
		else if (strstr(buffer, "usemtl") == buffer)
		{
			Replace('\n', '\0', buffer);
			currentMatName = &buffer[strlen("usemtl") + 1];
		}
	}
	fclose(fp);

	return true;
}

bool Model::LoadModel(const wchar_t * filename, int num)
{
	char filePath[256];
	std::vector<std::string>outMatList;
	modelData.index.clear();//�����ς肳����
	int pathTailPoint = 0;
	char loacal[dataSize + 1] = { 0x00 };
	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		filename,
		-1,
		loacal,
		dataSize + 1,
		NULL,
		NULL
	);
	for (int i = ret - 1; i >= 0; i--)
	{
		if (filename[i] == '/')
		{
			pathTailPoint = i;
			break;
		}
	}
	strncpy_s(filePath, loacal, pathTailPoint + 1);

	if (CreateMesh(outMatList, filePath, filename) == false)
	{
		return false;
	}
	if (MaterialLoad(outMatList, filePath) == false)
	{
		return false;
	}

	CreateModel(outMatList, filePath, filename, num);
	return true;
}

bool Model::MaterialLoad(std::vector<std::string> fileList, std::string filePath)
{
	char buffer[1024];

	for (auto matFileName : fileList)
	{
		FILE* fp = nullptr;
		std::string name = filePath;
		name += matFileName;

		fopen_s(&fp, name.c_str(), "r");

		if (fp == nullptr)
		{
			return false;
		}
		std::string currentMatName = "";
		while (fgets(buffer, 1024, fp) != nullptr)
		{
			//�}�e���A���O���[�v��
			if (strstr(buffer, "newmtl") == buffer)
			{
				Replace('\n', '\0', buffer);
				currentMatName = &buffer[strlen("newmtl") + 1];
			}
			else if (strstr(buffer, "map_Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::string texName = &buffer[strlen("map_Kd") + 1];
				// �t�@�C���p�X���݂ŕۑ�
				mMats[currentMatName].TextureName = filePath + texName;

				std::vector<std::string>split;
				Split('.', (char*)texName.c_str(), split);

				if (split.size() > 1)
				{
					mMats[currentMatName].TexKeyWord = split[0];
				}
				LoadTex(WCHAR_char::Change(mMats[currentMatName].TextureName.c_str()));
			}
			//Ambient�J���[
			else if (strstr(buffer, "Ka") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Ka") + 1], split);
				for (int i = 0; i < split.size(); i++)
				{
					mMats[currentMatName].Ambient[i] = atof(split[i].c_str());
				}
			}
			//�f�t���[�Y����[
			else if (strstr(buffer, "Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Kd") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					mMats[currentMatName].Diffuse[i] = atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Ks") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Ks") + 1], split);
				for (int i = 0; i < split.size(); i++)
				{
					mMats[currentMatName].Specular[i] = (float)atof(split[i].c_str());
				}
			}
		}
		fclose(fp);
	}
	return true;
}

void Model::DrawModel(int num, const Vector3 & pos, const Vector3 & angle, const Vector3& size)
{
	auto d = modelDataList[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
	d.matWorld = Matrix4::Identity;
	d.scale = size;
	d.matWorld = Matrix4::createTranslation(-d.ancPoint3D);
	
	d.matWorld *= Matrix4::createScale(d.scale);
	d.ancAngle.x = angle.x;
	d.ancAngle.y = angle.y;
	d.ancAngle.z = angle.z;
	d.matWorld *= Matrix4::RotateZ(d.ancAngle.z);
	d.matWorld *= Matrix4::RotateX(d.ancAngle.x);
	d.matWorld *= Matrix4::RotateY(d.ancAngle.y);
	
	d.matWorld *= Matrix4::createTranslation(Vector3(pos.x, -pos.y, pos.z));
	
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = d.matWorld*Camera::matView *matProjectionM;
	d.constBuff->Unmap(0, nullptr);


	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(modelPipeLineState.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(modelRootSignature.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		//�}�e���A���ݒ�
	for (auto index : mIndices)
	{
		SetMaterial(&mMats[index.first]);
	}
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { modelDescHeap.Get() };
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
			modelDescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(d.index.size(), 1, 0, 0, 0);
}

void Model::SetMaterial(OBJMat * mat)
{
	constMap->MatAmbinent = XMFLOAT4(mat->Ambient[0], mat->Ambient[1], mat->Ambient[2], 1);
	constMap->MatDiffuse = XMFLOAT4(mat->Diffuse[0], mat->Diffuse[1], mat->Diffuse[2], 1);
	constMap->MatSpecular = XMFLOAT4(mat->Specular[0], mat->Specular[1], mat->Specular[2], 1);
}

void Model::SetModelAncPoint(int num ,Vector3 ancPoint,Vector3 ancAngle)
{
	auto d = modelDataList[num];
	modelDataList.erase(num);
	d.ancPoint3D = ancPoint;
	d.ancAngle = ancAngle;
	modelDataList.emplace(num, d);
}

void Model::SetSize(const int num, const Vector3 size)
{
	auto d = modelDataList[num];
	modelDataList.erase(num);
	d.scale = size;
	modelDataList.emplace(num, d);
}

void Model::Split(char splitChar, char * buffer, std::vector<std::string>& out)
{
	int count = 0;
	if (buffer == nullptr)
	{
		return;
	}
	int startPoint = 0;

	while (buffer[count] != '\0')
	{
		if (buffer[count] == splitChar)
		{
			if (startPoint != count)
			{
				char splitStr[256] = { 0 };
				strncpy_s(splitStr, 256, &buffer[startPoint], count - startPoint);
				out.emplace_back(splitStr);
			}
			else
			{
				out.emplace_back("");
			}

			startPoint = count + 1;
		}
		count++;
	}
	if (startPoint != count)
	{
		char splitStr[256] = { 0 };
		strncpy_s(splitStr, 256, &buffer[startPoint], count - startPoint);
		out.emplace_back(splitStr);
	}
}

void Model::Replace(char searchChar, char replaceChar, char * buffer)
{
	int len = strlen(buffer);
	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == searchChar)
		{
			buffer[i] = replaceChar;
		}
	}
}

void Model::ParseVKeyWordTag(std::vector<Vector3>& outVertices, char * buff)
{
	std::vector<std::string>splitStrings;
	Split(' ', buff, splitStrings);

	int count = 0;
	float values[3] = { 0.0f };
	for (std::string str : splitStrings)
	{
		values[count] = atof(str.c_str());
		count++;
	}
	outVertices.push_back(Vector3(values[0], values[1], values[2]));
}

void Model::ParseFKeyWordTag(std::vector<VertexM>& outObjVert, std::map<std::string, int>& indexList, std::string currentMat, std::vector<Vector3>& vertices, std::vector<Vector3>& textures, std::vector<Vector3>& normals, char * buffer)
{
	int count = 0;
	int vertexInfo[3] =
	{
		-1,-1,-1,
	};
	std::vector<std::string>spaceSplit;
	Split(' ', buffer, spaceSplit);
	for (int i = 0; i < spaceSplit.size(); i++)
	{
		VertexM objVert;
		ParseShashKeyWordTag(vertexInfo, (char*)spaceSplit[i].c_str());
		for (int i = 0; i < 3; i++)
		{
			if (vertexInfo[i] == -1)
			{
				continue;
			}
			int id = vertexInfo[i];

			switch (i)
			{
			case 0:
				objVert.pos = vertices[id];
				break;
			case 1:
				objVert.uv = Vector2(textures[id].x, textures[id].y);
				break;
			case 2:
				objVert.normal = normals[id];
				break;
			default:
				break;
			}

		}
#define OPTIMIZATION (0)
#if OPTIMIZATION
		std::string key = "";
		for (int i = 0; i < 3; i++)
		{
			std::ostringstream sout;
			sout << std::setfill('0') << std::setw(5) << vertexInfo[i];
			key += sout.str();
		}
		if (indexList.count(key) > 0)
		{
			mIndices[currentMat].push_back(indexList[key]);
		}
		else
		{
			outObjVert.push_back(vertex);
			mIndices[currentMat].push_back(outObjVert.size() - 1);
			indexList[key] = outObjVert.size();
		}
#else
		//���_�o�b�t�@���X�g�֒ǉ�
		outObjVert.push_back(objVert);
		mIndices[currentMat].push_back(outObjVert.size() - 1);
		//�C���f�b�N�X�o�b�t�@�ɒǉ�
		modelData.index.push_back(outObjVert.size() - 1);
#endif


	}
	//�|���S���̍쐬�̒��_���Ԃ𔽓]����
	int size = modelData.index.size();
	int temp = modelData.index[size - 1];
	modelData.index[size - 1] = modelData.index[size - 3];
	modelData.index[size - 3] = temp;
}

void Model::ParseShashKeyWordTag(int * list, char * buffer)
{
	int counter = 0;
	std::vector<std::string>slashSplit;
	Split('/', buffer, slashSplit);
	for (std::string str : slashSplit)
	{
		if (str.size() > 0)
		{
			list[counter] = atoi(str.c_str()) - 1;
		}
		counter++;
	}
}
