#include "Particle.h"
#include"WCHAR_char.h"
#include <codecvt> 
#include"Camera.h"
const int spriteSRVCount = 512;//�e�N�X�`���ő喇��
ComPtr<ID3D12Resource>TexBuff[spriteSRVCount];
namespace {
	const int dataSize = 24;
}
Particle::Particle(ID3D12Device* device, ID3D12GraphicsCommandList*commandList):
	dev(device),cmdList(commandList),count(0)
{
	Init();
}

Particle::~Particle()
{
	dataListP.clear();
	texList.clear();
	countList.clear();
}

void Particle::Init()
{
	matProjectionP = Matrix4::Identity;
	matProjectionP.m[3][0] = -1.0f;
	matProjectionP.m[3][1] = 1.0f;
	matProjectionP.m[0][0] = 2.0f / Window::Window_Width;
	matProjectionP.m[1][1] = -2.0f / Window::Window_Height;
	InitGraphicPipeLine();
}

void Particle::Update()
{

	Matrix4 matBill = Matrix4::Identity;

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
	//�������s�����p�[�e�B�N���̍폜
	particles.remove_if([](ParticleStateus& x) {return x.frame >= x.numFrame; });
	//�S�p�[�e�B�N���X�V
	for (std::forward_list<ParticleStateus>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//�o�߃t���[����
		it->frame++;
		//���x�ɉ����x�����Z
		it->vel = it->vel + it->acc;
		//���x�ɂ��ړ�
		it->pos = it->pos + it->vel;

		//�J���[
		//it->color = it->color;
		//�X�P�[��
		float f = (float)it->numFrame / it->frame;
		//�X�P�[���̐��`�ۊ�
		it->scale = (it->eScale - it->sScale)/f;
		it->scale += it->sScale;
		

	}
	//���_�f�[�^���o�b�t�@��
	VertexP* vertMap = nullptr;
	result = dataP.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<ParticleStateus>::iterator it = particles.begin(); it != particles.end(); it++)
		{	
			vertMap->pos = it->pos;
			vertMap->color = it->color;
			vertMap->scale = it->scale;
			vertMap++;
		}
		dataP.vertBuff->Unmap(0, nullptr);
	}
	//�s��̓]��
	constMap = nullptr;
	result = dataP.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = Camera::matView *matProjectionP;
	constMap->matBill = matBill;
	dataP.constBuff->Unmap(0, nullptr);

}

void Particle::CreateBuff()
{
	//���_�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(dataP.vertex[0])*dataP.vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&dataP.vertBuff));

	VertexP* vertMap = nullptr;
	//�o�b�t�@�փf�[�^�𑗐M
	result = dataP.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, dataP.vertex.data(), sizeof(dataP.vertex[0])* dataP.vertex.size());
	dataP.vertBuff->Unmap(0, nullptr);

	
	//���_�o�b�t�@�r���[
	dataP.vbView.BufferLocation = dataP.vertBuff->GetGPUVirtualAddress();
	dataP.vbView.SizeInBytes = sizeof(dataP.vertex[0])*dataP.vertex.size();
	dataP.vbView.StrideInBytes = sizeof(dataP.vertex[0]);
	

	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&dataP.constBuff));

	XMMATRIX xx = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Window::Window_Width / Window::Window_Height,
		0.1f, 1000.0f
	);
	result = dataP.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	float xxx[4][4] =
	{
		{xx.r[0].m128_f32[0],xx.r[0].m128_f32[1],xx.r[0].m128_f32[2],xx.r[0].m128_f32[3]},
		{xx.r[1].m128_f32[0],xx.r[1].m128_f32[1],xx.r[1].m128_f32[2],xx.r[1].m128_f32[3]},
		{xx.r[2].m128_f32[0],xx.r[2].m128_f32[1],xx.r[2].m128_f32[2],xx.r[2].m128_f32[3]},
		{xx.r[3].m128_f32[0],xx.r[3].m128_f32[1],xx.r[3].m128_f32[2],xx.r[3].m128_f32[3]}
	};

	matProjectionP = Matrix4(xxx);
	constMap->mat = matProjectionP;
	dataP.constBuff->Unmap(0, nullptr);
}

void Particle::CreatePlane()
{
	dataP.vertex.clear();
	dataP.vertex.resize(vertCount);//�{���͂P
	////float size = 100.0f;
	////��O
	//for (int i = 0; i < dataP.vertex.size();i++)
	//{
	//	const float aida = 30.0f;
	//	dataP.vertex[i].pos.x = (float)rand()/RAND_MAX*aida - aida/2.0f;
	//	dataP.vertex[i].pos.y = (float)rand() / RAND_MAX * aida - aida / 2.0f;
	//	dataP.vertex[i].pos.z = (float)rand() / RAND_MAX * aida - aida / 2.0f;
 //   }
	
}

HRESULT Particle::InitGraphicPipeLine()
{
	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	//VS
	result = D3DCompileFromFile(
		L"ParticleVertexShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob
	);
	//GS
	result = D3DCompileFromFile(
		L"ParticleGeometryShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "gs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&gsBlob, &errorBlob
	);
	//PS
	result = D3DCompileFromFile(
		L"ParticlePixelShader.hlsl",//�V�F�[�_�[�t�@�C����
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
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		 {
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�O���t�B�b�N�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeLine{};
	gpipeLine.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeLine.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
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
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/
	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	//���Z����
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/
	//�u�����h�X�e�[�g�ɐݒ�
	gpipeLine.BlendState.RenderTarget[0] = blenddesc;
	//�[�x�o�b�t�@�t�H�[�}�b�g
	gpipeLine.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//�f�v�X�̏������݋֎~
	gpipeLine.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//���_���C�A�E�g�̐ݒ�
	gpipeLine.InputLayout.pInputElementDescs = inputLayout;
	gpipeLine.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`����O�p�`��
	gpipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
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
		IID_PPV_ARGS(&RootSignatureP));

	//�O���t�B�b�N�p�C�v���C���X�e�[�g�̐����ɃZ�b�g
	gpipeLine.pRootSignature = RootSignatureP.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeLine, IID_PPV_ARGS(&PipeLineStateP));
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&DescHeap));

	return S_OK;


}

void Particle::LoadTex(const wchar_t * filename)
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
		IID_PPV_ARGS(&TexBuff[count])
	);
	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = TexBuff[count]->WriteToSubresource(
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
		TexBuff[count].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(DescHeap->GetCPUDescriptorHandleForHeapStart(),
			count, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	texList.emplace(filename, TexBuff[count].Get());
	countList.emplace(filename, count);
	count++;
}

void Particle::CreateParticleData(const wchar_t * filename, const int& num)
{
	CreatePlane();
	
	std::vector<DirectVertexP>v;
	v.clear();
	v.resize(dataP.vertex.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i].pos.x = dataP.vertex[i].pos.x;
		v[i].pos.y = dataP.vertex[i].pos.y;
		v[i].pos.z = dataP.vertex[i].pos.z;
	}
	v.clear();
	

	dataP.texNum = countList[filename];
	CreateBuff();
	dataListP.emplace(num, dataP);
}

void Particle::DrawParticleBill(const int  num)
{
	auto d = dataListP[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineStateP.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignatureP.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { DescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			DescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h

	//cmdList->DrawInstanced(d.vertex.size(), 1, 0, 0);//���_�̂݃W�I���g���p
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(),particles.end()),1,0,0);//���_�̂݃W�I���g���p
	
}

void Particle::DrawParticleBillY(const int & num, const Vector3 & pos)
{
	Matrix4 matBill = Matrix4::Identity;
	auto d = dataListP[num];
	d.texNum = num;//�e�N�X�`���𕪂��Ă܂��I�I�I�I�I�I�I�I�I
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

	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->matBill =Camera::matView *matProjectionP;
	constMap->matBill = matBill;

	d.constBuff->Unmap(0, nullptr);


	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(PipeLineStateP.Get());
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(RootSignatureP.Get());
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { DescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//���_�o�b�t�@�Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);

	//�萔�o�b�t�@�̃Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			DescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawInstanced(d.vertex.size(), 1, 0, 0);//���_�̂݃W�I���g���p
}

void Particle::Add(int life, Vector3 pos, Vector3 vel, Vector3 acc, float sScale, float eScale,Vector4 color)
{
	if (std::distance(particles.begin(), particles.end())>=vertCount)
	{
		return;
	}
	particles.emplace_front();
	ParticleStateus& p = particles.front();
	//�l�̃Z�b�g
	p.pos = pos;
	p.vel = vel;
	p.acc = acc;
	p.sScale = sScale;
	p.eScale = eScale;
	p.color = color;
	p.numFrame = life;
}

ParticleEmitter::ParticleEmitter(ID3D12Device * device, ID3D12GraphicsCommandList * commandList)
{
    particle = new Particle(device,commandList);
}

ParticleEmitter::~ParticleEmitter()
{
	delete particle;
}

void ParticleEmitter::Load()
{
	particle->LoadTex(L"Resouse/kisikun.png");
	particle->CreateParticleData(L"Resouse/kisikun.png", 0);
}

void ParticleEmitter::Update()
{
	
	for (int i = 0; i < 10; i++)
	{
		const float rndVel = 1.5f;
		Vector3 pVel;
		pVel.x = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.y = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.z = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;

		Vector3 acc;
		const float rndAcc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rndAcc;
		r = (float)rand() / RAND_MAX * 0.2f;
		g = (float)rand() / RAND_MAX * 0.2f;
		b = (float)rand() / RAND_MAX * 0.2f;
		//���X�g�ɒǉ�
		particle->Add(60, pos, pVel, acc, 3.0f, 0.0f, Vector4(r,g,b,1.0f));
	}
	particle->Update();
	particle->DrawParticleBill(0);
}

void ParticleEmitter::SetPos(Vector3 pos)
{
    this->pos = pos;
}
