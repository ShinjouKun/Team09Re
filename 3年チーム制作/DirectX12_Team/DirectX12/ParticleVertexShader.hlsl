#include "ParticleShaderHeader.hlsli"
VSOutput VSmain(float4 pos:POSITION, float4 color : COLOR,float scale:TEXCOORD)
{
	VSOutput output;//�s�N�Z���V�F�[�_�ɓn���l
	output.color = color;
	output.svpos = pos;
	output.scale = scale;
	return output;
}