/**
 * @file VertexShader.hlsl
 */


cbuffer WorldBuffer : register(b3) {
	float4x4 world;
}

// カメラの情報
cbuffer CameraBuffer : register(b4) {
	float4x4 view;
	float4x4 proj;
}

struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

// 頂点シェーダからピクセルシェーダに渡す情報
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 wPos : TEXCOORD1;
};



VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);

	// 行列とベクトルの計算
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.wPos = VOUT.pos.xyz;

	// カメラ座標へ変換
	VOUT.pos = mul(VOUT.pos, view);

	// カメラの画角
	VOUT.pos = mul(VOUT.pos, proj);

	// UV情報
	VOUT.uv = VIN.uv;

	// ノーマライズ
	VOUT.normal = VIN.normal;

	return VOUT;
}

// EOF