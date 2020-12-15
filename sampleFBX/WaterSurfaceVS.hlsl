/**
 * @file WaterSurfaceVS.flsl
 */


// 頂点シェーダーの入力パラメータ
struct VS_IN
{
	float3 pos   : POSITION;	// 頂点座標
	float2 texel : TEXCOORD;	// テクセル
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
	float3 pos    : POSITION;
	float2 texel  : TEXCOORD;
};


VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	// 頂点シェーダーではそのまま渡す
	Out.pos = In.pos;
	Out.texel = In.texel;
	return Out;
}

// EOF