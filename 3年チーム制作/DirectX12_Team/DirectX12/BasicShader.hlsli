cbuffer cbuff0:register(b0)
{
	float4 color;//RGBA
	matrix mat;//3D���W�ϊ�
};

//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p
struct VSOutput
{
	float4 svpos : SV_POSITION;//���_���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;//uv�l
};