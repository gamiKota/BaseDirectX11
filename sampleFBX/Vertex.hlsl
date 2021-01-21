// 頂点シェーダ

//// グローバル
//cbuffer global : register(b0) {
//	matrix g_mWVP;
//	matrix g_mWorld;
//};

// カメラ
cbuffer global_camera : register(b0) {
	float4	g_cameraPos;	// 視点座標(ワールド空間)
	matrix	g_View;			// ビュー
	matrix	g_Proj;			// プロジェクション
};

// 描画オブジェクトの行列情報
cbuffer global_world : register(b3) {
	matrix	g_World;		// ワールド行列
	matrix	g_mTexture;		// テクスチャ行列
};

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float3	Normal		: NORMAL;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix WVP;
	float4 P = float4(input.Position, 1.0f);
	WVP = mul(g_World, g_View);
	WVP = mul(WVP, g_Proj);
	output.Position = mul(P, WVP);
	output.Pos4PS = mul(P, g_World).xyz;
	output.Normal = mul(float4(input.Normal, 0.0f), g_World).xyz;
	return output;
}
