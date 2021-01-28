struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
};


cbuffer ViewProj : register(b0)
{
	float4	g_cameraPos;	// 視点座標(ワールド空間)
	float4x4 view;
	float4x4 proj;
};
cbuffer World : register(b3)
{
	float4x4 world;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.wPos = VOUT.pos;
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);

	VOUT.uv = VIN.uv;

	VOUT.normal = mul(VIN.normal, (float3x3)world);

	VOUT.color = VIN.color;
	return VOUT;
}