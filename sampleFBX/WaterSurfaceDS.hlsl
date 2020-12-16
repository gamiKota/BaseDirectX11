
#define MAX_DROP 4


cbuffer global : register(b0) {
	matrix	g_WorldViewProj;	// ワールド×ビュー×射影行列
};


cbuffer globalFurface : register(b2) {
	float4	g_Furface[4];			// タイマー
};


//cbuffer global : register(b3) {
//	float4	g_amplitude[4];	// 振幅
//};
//
//
//cbuffer global : register(b4) {
//	float4	g_DropPos[4];	// 座標
//};



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


// ドメインシェーダーの出力パラメータ
struct DS_OUT
{
	float4 pos   : SV_POSITION;
	float2 texel : TEXCOORD0;
};


// *****************************************************************************************
// ドメインシェーダー
// *****************************************************************************************
[domain("quad")]
DS_OUT main(CONSTANT_HS_OUT In, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUT, 4> patch)
{
	DS_OUT Out;

	// 頂点座標
	float3 p1 = lerp(patch[1].pos, patch[0].pos, uv.x);
	float3 p2 = lerp(patch[3].pos, patch[2].pos, uv.x);
	float3 p3 = lerp(p1, p2, uv.y);
	Out.pos = mul(float4(p3, 1.0f), g_WorldViewProj);

	// テクセル
	float2 t1 = lerp(patch[1].texel, patch[0].texel, uv.x);
	float2 t2 = lerp(patch[3].texel, patch[2].texel, uv.x);
	float2 t3 = lerp(t1, t2, uv.y);
	Out.texel = t3;

	float amplitude = 0.f;
	float dx = 0.f;
	float dz = 0.f;
	float length = 0.f;
	float timer = 0.f;
	float PosY = 0.f;
	
	// 振動
	for (int i = 0; i < 4; ++i) {
		//if (g_amplitude[i] <= 0) {
		//	continue;
		//}
	
		// 影響力の計算
		amplitude = g_Furface[i].w;
		dx = (g_Furface[i].x - Out.texel.x) * (g_Furface[i].x - Out.texel.x);
		dz = (g_Furface[i].y - Out.texel.y) * (g_Furface[i].y - Out.texel.y);
		timer = g_Furface[i].z;
	
		// 距離で影響力を減算
		length = sqrt(dx + dz);
		length *= 100.f;
		amplitude -= length;
	
		if (amplitude < 0) {
			amplitude = 0;
			continue;
		}
	
		// 最終的な座標への加算
		PosY += amplitude * sin(2.f * 3.141592f * ((length / 14.f) - (timer / 1.f)));
	
		//break;
	}
	Out.pos.y += PosY;

	//float amplitude = g_amplitude.x;
	//float dx = (g_DropPos.x - Out.texel.x) * (g_DropPos.x - Out.texel.x);
	//float dz = (g_DropPos.y - Out.texel.y) * (g_DropPos.y - Out.texel.y);
	//float length = sqrt(dx + dz);
	//length *= 100.f;
	//amplitude -= length;
	//if (amplitude < 0) {
	//	amplitude = 0;
	//}
	//Out.pos.y += amplitude * sin(2.f * 3.141592f * ((length / 14.f) - (g_timer.x / 1.f)));

	//Out.pos.y += 2.f * sin(2.f * g_timer);
	//dx = ((g_Wave[20][20].position.x + g_Wave[z][x].position.x) * (g_Wave[20][20].position.x + g_Wave[z][x].position.x));
	//dz = ((g_Wave[20][20].position.z + g_Wave[z][x].position.z) * (g_Wave[20][20].position.z + g_Wave[z][x].position.z));
	//length = sqrtf(dx + dz);
	//Out.pos.y = g_Wave[z][x].amplitude * sinf(2.f * PI * ((length / WAVE_LENGTH) - (g_Wave[z][x].time / WAVE_CYCLE)));
	
	return Out;
}