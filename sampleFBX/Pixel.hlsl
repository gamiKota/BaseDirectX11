// ピクセルシェーダ

// カメラ
cbuffer global_camera : register(b0) {
	float4	g_cameraPos;	// 視点座標(ワールド空間)
	matrix	g_View;			// ビュー
	matrix	g_Proj;			// プロジェクション
};

// ライト
cbuffer global_light : register(b1) {
	float4	g_lightDir;			// 光源方向(ワールド空間)
	float4	g_lightAmbient;		// 環境光
	float4	g_lightDiffuse;		// 拡散光
	float4	g_lightSpecular;	// 鏡面反射光
};

// ライトのON/OFF
cbuffer global_lightsetting : register(b2) {
	float4 g_lightSetting;
};

// 描画オブジェクトの行列情報
cbuffer global_world : register(b3) {
	matrix	g_World;		// ワールド行列
	matrix	g_mTexture;		// テクスチャ行列
};

// 描画オブジェクトのマテリアル情報
cbuffer global_material : register(b4) {
	float4	g_Ambient;			// 環境色
	float4	g_Diffuse;			// 拡散色
	float4	g_Specular;			// 鏡面反射色
	float4	g_Emissive;			// 発光色
};

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};

float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;

	if (Alpha <= 0.0f) discard;

	if (g_lightDir.x != 0.0f || g_lightDir.y != 0.0f || g_lightDir.z != 0.0f) {
		// 光源有効
		float3 L = normalize(-g_lightDir.xyz);				// 光源へのベクトル
		float3 N = normalize(input.Normal);					// 法線ベクトル
		float LN = dot(L, N);
		if (LN < 0.0f) {
			LN = -LN;
			N = -N;
		}
		float3 V = normalize(g_cameraPos.xyz - input.Pos4PS);	// 視点へのベクトル
		float3 H = normalize(L + V);						// ハーフベクトル
		Diff = g_lightAmbient.rgb * g_Ambient.rgb + g_lightDiffuse.rgb *
			Diff * saturate(LN);						// 拡散色 + 環境色
		float3 Spec = g_lightSpecular.rgb * g_Specular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a);				// 鏡面反射色
		Diff += Spec;
	}

	Diff += g_Emissive.rgb;										// 発光色

	return float4(Diff, Alpha);
}
