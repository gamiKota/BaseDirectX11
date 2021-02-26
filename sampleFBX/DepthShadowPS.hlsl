
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 lightPos : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t7);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	
	// 光源から見た座標をUV値で取得
	float3 shadowUV = PIN.lightPos.xyz / PIN.lightPos.w;
	// 深度値を取得
	float depth = PIN.lightPos.z / PIN.lightPos.w;
	// テクスチャの深度値を取得
	float shadowDepth = shadow.Sample(samp, shadowUV.xy).r;

	//// 最大深度傾斜
	//float  maxDepthSlope = max(abs(ddx(shadowUV.z)), abs(ddy(shadowUV.z)));
	//float  shadowThreshold = 1.0f;      // シャドウにするかどうかの閾値
	//float  bias = 0.01f;				// 固定バイアスです
	//float  slopeScaledBias = 0.001f;     // 深度傾斜
	//float  depthBiasClamp = 0.1f;		// バイアスクランプ値
	//float  shadowBias = bias + slopeScaledBias * maxDepthSlope;
	//shadowBias = min(shadowBias, depthBiasClamp);
	//float3 shadowColor = float3(0.25f, 0.25f, 0.25f);
	////shadowThreshold = shadow.SampleCmpLevelZero(samp, shadowUV.xy, shadowUV.z - shadowBias);
	////shadowColor = lerp(shadowColor, float3(1.0f, 1.0f, 1.0f), shadowThreshold);
	//shadowColor = lerp(shadowColor, float3(1.0f, 1.0f, 1.0f), shadowDepth);
	//color.rgb *= shadowColor;

	// 描画する深度値の方が大きい(カメラから遠い)なら影になる
	// テクスチャの深度値と描画の深度値が似たような距離の場合、
	// 計算の誤差で影にならない部分が影とみなされる(シャドウアクネ)。
	// 誤差を考慮して深度値を補正してあげる(バイアス)
	// 影の減衰処理
	if (depth > shadowDepth + 0.00001f) {
		color.rgb *= 0.3f;
	}

	return color;
}