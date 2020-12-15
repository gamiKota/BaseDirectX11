/**
 * @file WaterSurfaceConstHS.hlsl
 */


 // 頂点シェーダーの出力パラメータ
struct VS_OUT
{
	float3 pos    : POSITION;
	float2 texel  : TEXCOORD0;
};

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT
{
	float Edges[4] : SV_TessFactor;        // パッチのエッジのテッセレーション係数
	float Inside[2] : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};

// ハルシェーダーのコントロール ポイント フェーズ用の出力パラメータ
struct HS_OUT
{
	float3 pos   : POSITION;
	float2 texel : TEXCOORD0;
};

cbuffer Param : register(b1)
{
	float cbEdgeFactor;		//三角形の辺の分割量の指定
	float cbInsideFactor;	//三角形の内部の分割量の指定
};


// パッチ定数関数の定義
#define NUM_CONTROL_POINTS 4


// *****************************************************************************************
// ハルシェーダーのパッチ定数フェーズ
// *****************************************************************************************
CONSTANT_HS_OUT ConstantsHS_Main(InputPatch<VS_OUT, 4> ip, uint PatchID : SV_PrimitiveID)
{
	CONSTANT_HS_OUT Out;

	// 定数バッファの値をそのまま渡す
	Out.Edges[0] = Out.Edges[1] = Out.Edges[2] = Out.Edges[3] = cbEdgeFactor;  // パッチのエッジのテッセレーション係数
	Out.Inside[0] = cbInsideFactor;  // パッチ内部の横方法のテッセレーション係数
	Out.Inside[1] = cbInsideFactor;  // パッチ内部の縦方法のテッセレーション係数

	return Out;
}


// *****************************************************************************************
// ハルシェーダーのコントロール ポイント フェーズ
// *****************************************************************************************
[domain("quad")]							// テッセレートするメッシュの形状を指定する
[partitioning("integer")]					// パッチの分割に使用するアルゴリズムを指定する
[outputtopology("triangle_ccw")]			// メッシュの出力方法を指定する
[outputcontrolpoints(4)]					// ハルシェーダーのコントロール ポイント フェーズがコールされる回数
[patchconstantfunc("ConstantsHS_Main")]     // 対応するハルシェーダーのパッチ定数フェーズのメイン関数
HS_OUT main( InputPatch<VS_OUT, 4> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
	HS_OUT Out;
	
	// そのまま渡す
	Out.pos = In[i].pos;
	Out.texel = In[i].texel;
	return Out;
}


// EOF