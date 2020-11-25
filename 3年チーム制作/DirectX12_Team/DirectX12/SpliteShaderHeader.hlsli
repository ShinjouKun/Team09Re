cbuffer cbuff0:register(b0)
{
	float4 color;//RGBA
	matrix mat;//3D座標変換
};

//頂点シェーダからピクセルシェーダへのやり取りに使用
struct VSOutput
{
	float4 svpos : SV_POSITION;//システム用頂点座標
	float2 uv:TEXCOORD;//uv値
};