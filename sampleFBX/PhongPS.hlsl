/**
 * @file PhongPS.hlsl
 */


 // 頂点シェーダから渡されるデータ
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
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


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color;
	color = tex.Sample(samp, PIN.uv);

	// 影の計算
	// 光源のベクトルと法線の内積から暗さを計算する
	// 光源のベクトルを反転(-の値を掛ける)して、正しい見た目になるように計算する
	float3 L = normalize(-LightDir.xyz);
	float3 N = normalize(PIN.normal);

	// 内積
	float d = dot(N, L);

	// 1～0の範囲に補正する
	d = d * 0.5f + 0.5f;

	// 照り返し(反射の計算)
	float3 V = PIN.wPos.xyz - cameraPos.xyz;
	// reflect... 第二引数で渡した法線のベクトルから、第一引数のベクトルがどのように反射するか計算
	float3 R = reflect(V, N);
	R = normalize(R);
	float s = dot(L, R);
	// スペキュラーは1～0の範囲にない値を無視する
	// saturate... 引数を0～1の範囲に丸める
	s = saturate(s);
	// 照り返し部分はより強く際立つように計算結果を乗算する
	s = pow(s, 50);


	float4 diffuseLight		= LightDiffuse;		//!< 光源の色
	float4 ambientLight		= LightAmbient;		//!< 周りから反射した色
	float4 specularLight	= LightSpecular;	//!< 輝いて見える光の色
	float4 materialDiffuse	= g_Diffuse;		//!< 物体が跳ね返しやすい光
	float4 materialAmbient	= g_Ambient;		//!< 物体が跳ね返しやすい環境光
	float4 materialSpecular = g_Specular;		//!< 物体が金属っぽいか(反射光の係数)

	// 光マテリアルを考慮した影つけ
	float3 diffuse = (float3)(diffuseLight * materialDiffuse);
	float3 ambient = (float3)(ambientLight * materialAmbient);
	float3 specular = (float3)(specularLight * materialSpecular);
	float3 lambert = diffuse * d + ambient;
	float3 phong = specular * s;

	color.rgb *= lambert + phong;

	return color;
}


// EOF