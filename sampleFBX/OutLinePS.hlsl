// FBX用ピクセルシェーダ (FbxModelPixel.hlsl)

// グローバル
cbuffer global : register(b0) {
	matrix	g_WorldViewProj;	// ワールド×ビュー×射影行列
	matrix	g_World;			// ワールド行列
	float4	g_cameraPos;		// 視点座標(ワールド空間)
	float4	g_lightDir;			// 光源方向(ワールド空間)

	// メインライトの値
	float4	g_lightAmbient;		// 環境光
	float4	g_lightDiffuse;		// 拡散光
	float4	g_lightSpecular;	// 鏡面反射光
};

// マテリアル
cbuffer global2 : register(b1) {
	// モデルごとの値
	float4	g_Ambient;			// 環境色
	float4	g_Diffuse;			// 拡散色
	float4	g_Specular;			// 鏡面反射色
	float4	g_Emissive;			// 発光色
};


// パラメータ
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

//
// ピクセルシェーダ
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	return float4(0.f, 0.f, 0.f, 1.f);
}
