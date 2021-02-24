struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 lightPos : TEXCOORD1;
};

cbuffer World : register(b0)
{
	float4x4 world;
};
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};
cbuffer LightScreenMat : register(b2)
{
	float4x4 lightVPS;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.lightPos = mul(VOUT.pos, lightVPS);
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);

	VOUT.uv = VIN.uv;


	return VOUT;
}