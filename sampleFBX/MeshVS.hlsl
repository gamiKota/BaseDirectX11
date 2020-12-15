// 頂点シェーダ

// グローバル
cbuffer global : register(b0) {
	matrix g_mWVP;
	matrix g_mWorld;
	matrix g_mTexture;
};

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float3	Normal		: NORMAL;
	float4	Diffuse		: COLOR;
	float2	TexCoord	: TEXCOORD;
};


struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 P = float4(input.Position, 1.0f);
	output.Position = mul(P, g_mWVP);
	output.Pos4PS = mul(P, g_mWorld).xyz;
	output.Normal = mul(float4(input.Normal, 0.0f), g_mWorld).xyz;
	output.Diffuse = input.Diffuse;
	output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	//output.TexCoord = mul(float4(input.Normal.xy, 0.f, 1.0f), g_mTexture).xy;
	//output.TexCoord = mul(float4(input.Diffuse.xy, 0.f, 1.f), g_mTexture).xy;
	//output.TexCoord = float2(1, 1);

	return output;
}
