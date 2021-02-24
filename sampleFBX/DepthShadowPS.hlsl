
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 lightPos : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	
	// 光源から見た座標をUV値で取得
	float2 shadowUV = PIN.lightPos.xy / PIN.lightPos.w;
	// 深度値を取得
	float depth = PIN.lightPos.z / PIN.lightPos.w;
	// テクスチャの深度値を取得
	float shadowDepth = shadow.Sample(samp, shadowUV).r;

	// 描画する深度値の方が大きい(カメラから遠い)なら影になる
	// テクスチャの深度値と描画の深度値が似たような距離の場合、
	// 計算の誤差で影にならない部分が影とみなされる(シャドウアクネ)。
	// 誤差を考慮して深度値を補正してあげる(バイアス)
	if (depth > shadowDepth + 0.001f) {
		color.rgb *= 0.3f;
	}

	return color;
}