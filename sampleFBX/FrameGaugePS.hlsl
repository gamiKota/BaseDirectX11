struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float2 NormalTex : TEXCOORD2;
};

cbuffer Material : register(b4) {
	float4	g_Ambient;		// アンビエント色
	float4	g_Diffuse;		// ディフューズ色
	float4	g_Specular;		// スペキュラ色
	float4	g_Emissive;		// エミッシブ色

	float4	g_inDiffuse;	// ディヒューズ色
}

cbuffer Player : register(b5) {
	float4	g_playerPos;	// 座標
	float	g_playerHpRate;	// Hpの割合
}

Texture2D tex : register(t0);
Texture2D frameTex : register(t3);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target
{
	float hp = g_playerHpRate;

	float4 color = tex.Sample(samp, PIN.uv) + frameTex.Sample(samp, PIN.NormalTex);
	if (color.a > 0.f) {
		color = g_Diffuse;
	}
	else {
		if (PIN.uv.x <= hp) {
			color = g_Diffuse;
		}
		else {
			color = g_inDiffuse;
		}
	}
	return color;
}
