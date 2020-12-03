/**
 * @file VertexShader.hlsl
 */

// シェーダではブレークポイントは利用できない
// 主なデバッグ方法は各種パラメータを色情報として、画面に出力してあげる
// ただし、出力できる色情報は0.0～1.0の範囲になるので、
// 1より大きいデータは最大値で割るなどして、0.0～1.0の範囲に正規化する必要がある

// グローバル
//cbuffer global : register(b0) {
//	matrix	g_WorldViewProj;	// ワールド×ビュー×射影行列
//	matrix	g_World;			// ワールド行列
//	float4	g_cameraPos;		// 視点座標(ワールド空間)
//	float4	g_lightDir;			// 光源方向(ワールド空間)
//	float4	g_lightAmbient;		// 環境光
//	float4	g_lightDiffuse;		// 拡散光
//	float4	g_lightSpecular;	// 鏡面反射光
//};

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

	// 法線の情報をピクセルシェーダに渡す
	// ※一度ワールド座標に変換する
	// 法線は移動しないので、回転する成分のみで計算する
	//VOUT.normal = mul(VIN.normal, (float3x3)world);
	VOUT.normal = VIN.normal;

	return VOUT;

	//VS_OUTPUT VOUT;
	//VOUT.pos = float4(VIN.pos, 1);
	//
	//// 行列とベクトルの計算
	//VOUT.Pos = mul(VOUT.pos, g_World);
	//
	//VOUT.Pos = mul(VOUT.pos, g_WorldViewProj);
	//
	//VOUT.Tex = input.Tex;
	//VOUT.Normal = mul(vSkinned.Norm, (float3x3)g_World);
	//VOUT.PosForPS = mul(vSkinned.Pos, g_World).xyz;
	//
	//return VOUT;
}

// EOF