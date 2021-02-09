// 2D用頂点シェーダ

// グローバル
cbuffer camera : register(b0)
{
	float4	g_cameraPos;	// 視点座標(ワールド空間)
	float4x4 view;
	float4x4 proj;
	float4x4 view2D;
	float4x4 proj2D;
};
cbuffer world : register(b3) {
	matrix g_mWorld;
	matrix g_mTexture;
};

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float4	Diffuse		: COLOR0;
	float2	TexCoord	: TEXCOORD0;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
	float2	NormalTex	: TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 P = mul(float4(input.Position, 1.0f), g_mWorld);
	P = mul(P, view2D);
	output.Position = mul(P, proj2D);
	output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	output.Diffuse = input.Diffuse;
	output.NormalTex = input.TexCoord;
	return output;
}
