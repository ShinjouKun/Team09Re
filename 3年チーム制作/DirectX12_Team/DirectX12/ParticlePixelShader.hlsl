#include"ParticleShaderHeader.hlsli"

Texture2D<float4>tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
float4 PSmain(GSOutput input):SV_TARGET
{
	//�W�I���g��
	return tex.Sample(smp,input.uv)*input.color;
	//return input.color;
}