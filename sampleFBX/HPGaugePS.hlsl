struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
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

cbuffer Character : register(b6) {
	float4	g_charHp;	// Hpの割合
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = float4(0.3f, 0.3f, 1.f, 1.f);
	color *= g_Diffuse;
	if (color.a <= 0.f ||
		(PIN.uv.x > 0.62f - (PIN.uv.y * 0.15f) && PIN.uv.y > 0.6f)) {
		discard;
	}
	if (PIN.uv.x < 0.01f || PIN.uv.x > 0.94f ||
		PIN.uv.y < 0.1f || PIN.uv.y > 0.9f ||
		(PIN.uv.x > 0.6f - (PIN.uv.y * 0.15f) && PIN.uv.y > 0.5f)) {
		color = 1.f;
	}
	else if (PIN.uv.x >= g_charHp.x - (PIN.uv.y * 0.15f) ||
		PIN.uv.x > 0.6f - (PIN.uv.y * 0.15f) && PIN.uv.y > 0.5f) {
		discard;
	}

	return color;
}

// EOF