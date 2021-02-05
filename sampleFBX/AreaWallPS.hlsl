/**
 * @file PhongPS.hlsl
 */


 // 頂点シェーダから渡されるデータ
struct PS_IN {
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float4 color	: TEXCOORD2;
	float4 wPos		: TEXCOORD3;
};


cbuffer Camera : register(b0) {
	float4	cameraPos;	// 視点座標(ワールド空間)
	float4x4 view;
	float4x4 proj;
};
cbuffer Light : register(b1) {
	float4	LightDir;			// 光源方向
	float4	LightAmbient;		// 光源色(アンビエント)
	float4	LightDiffuse;		// 光源色(ディフューズ)
	float4	LightSpecular;		// 光源色(スペキュラ)
};
cbuffer World : register(b3) {
	float4x4 mtxWorld;
	float4x4 mtxTexture;
};
cbuffer Material : register(b4) {
	float4	g_Ambient;	// アンビエント色
	float4	g_Diffuse;	// ディフューズ色
	float4	g_Specular;	// スペキュラ色
	float4	g_Emissive;	// エミッシブ色
}
cbuffer Player : register(b5) {
	float4	g_playerPos;	// 座標
}


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 totalDiffuse = float4(0.f, 0.f, 0.f, 0.f);
	float PI = 3.1415926535;	// 円周率
	float rad1 = PI / 180.f;	// 1°のラジアン角

	// 描画するピクセルから点光源までのベクトル
	float3 dir = g_playerPos.xyz - PIN.wPos.xyz;
	float len = length(dir);
	dir = normalize(dir);

	// 内積から明るさを計算
	float d = dot(PIN.normal, dir);
	d = d * 0.5f + 0.5f;

	// 距離に応じて光を弱くする(減衰)
	float attenuation = saturate(1.f / (1.f + 20 * len + 30 * len * len));
	d *= attenuation;
	totalDiffuse += d;

	// 最終的な明るさを計算
	color.rgb *= totalDiffuse.rgb;
	//float len2 = length(totalDiffuse);
	//if (len) {
	//
	//}
	discard;


	return color;
}


// EOF