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

cbuffer camera : register(b0)
{
	float4	g_cameraPos;
	float4x4 view;
	float4x4 proj;
};
cbuffer Light : register(b1) {
	float4	LightDir;			// 光源方向
	float4	LightAmbient;		// 光源色(アンビエント)
	float4	LightDiffuse;		// 光源色(ディフューズ)
	float4	LightSpecular;		// 光源色(スペキュラ)
	float4x4 LightView;
	float4x4 LightProj;
	float4x4 lightVPS;
};
cbuffer world : register(b3) {
	float4x4 g_mWorld;
	float4x4 g_mTexture;
};
cbuffer LightScreen : register(b7) {
}

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, g_mWorld);
	VOUT.lightPos = mul(VOUT.pos, lightVPS);
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);

	VOUT.uv = VIN.uv;


	return VOUT;
}