struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
};


cbuffer Character : register(b6) {
	float4	g_charHp;	// Hp‚ÌŠ„‡
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = float4(0.3f, 0.3f, 1.f, 1.f);


	if (PIN.uv.x >= g_charHp.x) {
		discard;
	}

	return color;
}

// EOF