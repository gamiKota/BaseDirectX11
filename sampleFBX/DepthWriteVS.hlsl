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

cbuffer camera : register(b0)
{
	float4	g_cameraPos;	// 視点座標(ワールド空間)
	float4x4 view;
	float4x4 proj;
};
cbuffer Light : register(b1) {
	float4	LightDir;			// 光源方向
	float4	LightAmbient;		// 光源色(アンビエント)
	float4	LightDiffuse;		// 光源色(ディフューズ)
	float4	LightSpecular;		// 光源色(スペキュラ)
	float4x4 LightView;
	float4x4 LightProj;
	float4x4 LightScreenMat;
};
cbuffer World : register(b3) {
	float4x4 mtxWorld;
	float4x4 mtxTexture;
};

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, mtxWorld);
	VOUT.pos = mul(VOUT.pos, LightView);
	VOUT.pos = mul(VOUT.pos, LightProj);
	// 深度値はカメラから見た奥行きの情報のため、
	// ワールド座標ではなく、ビュー座標を利用する
	VOUT.shadowPos = VOUT.pos;

	VOUT.uv = VIN.uv;

	return VOUT;
}