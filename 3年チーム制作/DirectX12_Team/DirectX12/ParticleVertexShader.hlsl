#include "ParticleShaderHeader.hlsli"
VSOutput VSmain(float4 pos:POSITION, float4 color : COLOR,float scale:TEXCOORD)
{
	VSOutput output;//ピクセルシェーダに渡す値
	output.color = color;
	output.svpos = pos;
	output.scale = scale;
	return output;
}