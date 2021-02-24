struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	// 深度値を書き込むためにはピクセルシェーダで処理しないといけない。
	// でも、深度値(座標)の情報は頂点シェーダでしか扱ってないので、
	// 渡せるようにする
	float4 shadowPos : TEXCOORD1;
};

cbuffer World : register(b0)
{
	float4x4 world;
};
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.pos = mul(VOUT.pos, view);
	VOUT.pos = mul(VOUT.pos, proj);
	// 深度値はカメラから見た奥行きの情報のため、
	// ワールド座標ではなく、ビュー座標を利用する
	VOUT.shadowPos = VOUT.pos;

	VOUT.uv = VIN.uv;

	return VOUT;
}