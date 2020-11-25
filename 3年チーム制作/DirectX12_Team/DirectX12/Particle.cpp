#include "Particle.h"
#include"WCHAR_char.h"
#include <codecvt> 
#include"Camera.h"
const int spriteSRVCount = 512;//テクスチャ最大枚数
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
	//寿命が尽きたパーティクルの削除
	particles.remove_if([](ParticleStateus& x) {return x.frame >= x.numFrame; });
	//全パーティクル更新
	for (std::forward_list<ParticleStateus>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//経過フレーム数
		it->frame++;
		//速度に加速度を加算
		it->vel = it->vel + it->acc;
		//速度による移動
		it->pos = it->pos + it->vel;

		//カラー
		//it->color = it->color;
		//スケール
		float f = (float)it->numFrame / it->frame;
		//スケールの線形保管
		it->scale = (it->eScale - it->sScale)/f;
		it->scale += it->sScale;
		

	}
	//頂点データをバッファへ
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
	//行列の転送
	constMap = nullptr;
	result = dataP.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = Camera::matView *matProjectionP;
	constMap->matBill = matBill;
	dataP.constBuff->Unmap(0, nullptr);

}

void Particle::CreateBuff()
{
	//頂点バッファ作成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(dataP.vertex[0])*dataP.vertex.size()),//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&dataP.vertBuff));

	VertexP* vertMap = nullptr;
	//バッファへデータを送信
	result = dataP.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, dataP.vertex.data(), sizeof(dataP.vertex[0])* dataP.vertex.size());
	dataP.vertBuff->Unmap(0, nullptr);

	
	//頂点バッファビュー
	dataP.vbView.BufferLocation = dataP.vertBuff->GetGPUVirtualAddress();
	dataP.vbView.SizeInBytes = sizeof(dataP.vertex[0])*dataP.vertex.size();
	dataP.vbView.StrideInBytes = sizeof(dataP.vertex[0]);
	

	//定数バッファ
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
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
	result = dataP.constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
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
	dataP.vertex.resize(vertCount);//本来は１
	////float size = 100.0f;
	////手前
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
	//頂点シェーダーの読み込みとコンパイル
	//VS
	result = D3DCompileFromFile(
		L"ParticleVertexShader.hlsl",//シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"VSmain", "vs_5_0",//エントリーポイント名,シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&vsBlob, &errorBlob
	);
	//GS
	result = D3DCompileFromFile(
		L"ParticleGeometryShader.hlsl",//シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "gs_5_0",//エントリーポイント名,シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&gsBlob, &errorBlob
	);
	//PS
	result = D3DCompileFromFile(
		L"ParticlePixelShader.hlsl",//シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"PSmain", "ps_5_0",//エントリーポイント名,シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&psBlob, &errorBlob
	);

	//シェーダーのエラー内容の確認
	//errorBlobからエラーを受け取りstringにする
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	//頂点レイアウト
	//頂点シェーダーに渡すようの頂点データを整える
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

	//グラフィックパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeLine{};
	gpipeLine.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeLine.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeLine.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクとラスタライズステート設定
	gpipeLine.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//	標準
	gpipeLine.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeLine.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//背面カリングしない
	//深度テスト
	gpipeLine.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//gpipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書き
	gpipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準

	//共通設定
	blenddesc.BlendEnable = true;//ブレンド有効
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソース値１００％
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デスト値０％
	//半透明
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/
	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	//減算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/
	//ブレンドステートに設定
	gpipeLine.BlendState.RenderTarget[0] = blenddesc;
	//深度バッファフォーマット
	gpipeLine.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//デプスの書き込み禁止
	gpipeLine.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//頂点レイアウトの設定
	gpipeLine.InputLayout.pInputElementDescs = inputLayout;
	gpipeLine.InputLayout.NumElements = _countof(inputLayout);
	//図形の形状を三角形に
	gpipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//その他設定
	gpipeLine.NumRenderTargets = 1;//描画対象一つ
	gpipeLine.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeLine.SampleDesc.Count = 1;
	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//サンプラー設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//ルートシグネスチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//バージョン時動判定
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	//ルートシグネスチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&RootSignatureP));

	//グラフィックパイプラインステートの生成にセット
	gpipeLine.pRootSignature = RootSignatureP.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeLine, IID_PPV_ARGS(&PipeLineStateP));
	//デスクリプタヒープを生成
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
	if (itr != texList.end())//既に登録済なら
	{
		return;
	}
	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	const Image* img = scratchImg.GetImage(0, 0, 0);//生データ抽出
	//リソース設定
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	//テクスチャ用バッファの作成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ指定用
		nullptr,
		IID_PPV_ARGS(&TexBuff[count])
	);
	//テクスチャバッファにデータを転送
	result = TexBuff[count]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);
	//シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//２ｄテクスチ
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
	d.texNum = num;//テクスチャを分けてます！！！！！！！！！
	//ビューポート、シザー矩形
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//パイプライン設定
	cmdList->SetPipelineState(PipeLineStateP.Get());
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(RootSignatureP.Get());
	//プリミティブ形状
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	//スプライト描画コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { DescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//頂点バッファセット
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);
	//定数バッファのセット
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//シェーダリソースビューセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			DescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画コマンド

	//cmdList->DrawInstanced(d.vertex.size(), 1, 0, 0);//頂点のみジオメトリ用
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(),particles.end()),1,0,0);//頂点のみジオメトリ用
	
}

void Particle::DrawParticleBillY(const int & num, const Vector3 & pos)
{
	Matrix4 matBill = Matrix4::Identity;
	auto d = dataListP[num];
	d.texNum = num;//テクスチャを分けてます！！！！！！！！！
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

	//行列の転送
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->matBill =Camera::matView *matProjectionP;
	constMap->matBill = matBill;

	d.constBuff->Unmap(0, nullptr);


	//ビューポート、シザー矩形
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
	//パイプライン設定
	cmdList->SetPipelineState(PipeLineStateP.Get());
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(RootSignatureP.Get());
	//プリミティブ形状
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//スプライト描画コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { DescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//頂点バッファセット
	cmdList->IASetVertexBuffers(0, 1, &d.vbView);

	//定数バッファのセット
	cmdList->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//シェーダリソースビューセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			DescHeap->GetGPUDescriptorHandleForHeapStart(),
			d.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画コマンド
	cmdList->DrawInstanced(d.vertex.size(), 1, 0, 0);//頂点のみジオメトリ用
}

void Particle::Add(int life, Vector3 pos, Vector3 vel, Vector3 acc, float sScale, float eScale,Vector4 color)
{
	if (std::distance(particles.begin(), particles.end())>=vertCount)
	{
		return;
	}
	particles.emplace_front();
	ParticleStateus& p = particles.front();
	//値のセット
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
		//リストに追加
		particle->Add(60, pos, pVel, acc, 3.0f, 0.0f, Vector4(r,g,b,1.0f));
	}
	particle->Update();
	particle->DrawParticleBill(0);
}

void ParticleEmitter::SetPos(Vector3 pos)
{
    this->pos = pos;
}
