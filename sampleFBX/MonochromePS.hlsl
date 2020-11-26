/**
 * MonochromePS.hlsl
 */

Texture2D    g_texture		: register(t0);	// テクスチャ
SamplerState g_sampler		: register(s0);	// サンプラ

// パラメータ
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

//
// ピクセルシェーダ
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 color;
	color = g_texture.Sample(g_sampler, input.Tex);
	float mono = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
	color.rgb = mono;

	return color;
}

// EOF