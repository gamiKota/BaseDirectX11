// 2D用ピクセルシェーダ

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

cbuffer world : register(b3) {
	matrix g_mWorld;
	matrix g_mTexture;
};

cbuffer Material : register(b4) {
	float4	g_Ambient;	// アンビエント色
	float4	g_Diffuse;	// ディフューズ色
	float4	g_Specular;	// スペキュラ色
	float4	g_Emissive;	// エミッシブ色
}

Texture2D    g_texture : register(t0);	// テクスチャ
SamplerState g_sampler : register(s0);	// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = input.Diffuse;
	Color *= g_texture.Sample(g_sampler, input.TexCoord);
	Color *= g_Diffuse;
	if (Color.a <= 0.f) {
		discard;
	}
	return Color;
}
