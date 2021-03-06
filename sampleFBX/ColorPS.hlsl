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

// デバッグ用ヒットボックスはテクスチャが設定されてないので、
// ディヒューズが反映されない

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	if (color.a > 0.f) {
		color = g_Diffuse;
	}
	return color;
}
