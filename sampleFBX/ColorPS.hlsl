struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

cbuffer Material : register(b4) {
	float4	g_Ambient;	// アンビエント色
	float4	g_Diffuse;	// ディフューズ色
	float4	g_Specular;	// スペキュラ色
	float4	g_Emissive;	// エミッシブ色
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	return g_Diffuse;
}
