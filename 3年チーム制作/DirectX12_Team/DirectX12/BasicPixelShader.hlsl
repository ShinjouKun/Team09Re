#include"BasicShader.hlsli"

Texture2D<float4>tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
float4 PSmain(VSOutput input) :SV_TARGET
{

	float3 light = normalize(float3(1,-1,1));//�E���������̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//return float4(0,1,0,1);
	// return texcolor;
	//return float4((texcolor.rgb*col.rgb)*brightness, texcolor.a * col.a)*color;//�P�x��RGB�ɑ�����ďo��
	return float4(texcolor.rgb*brightness, texcolor.a)*color;//�P�x��RGB�ɑ�����ďo��
}