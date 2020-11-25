#include"BasicShader.hlsli"

Texture2D<float4>tex:register(t0);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー
float4 PSmain(VSOutput input) :SV_TARGET
{

	float3 light = normalize(float3(1,-1,1));//右下奥向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//return float4(0,1,0,1);
	// return texcolor;
	//return float4((texcolor.rgb*col.rgb)*brightness, texcolor.a * col.a)*color;//輝度をRGBに代入して出力
	return float4(texcolor.rgb*brightness, texcolor.a)*color;//輝度をRGBに代入して出力
}