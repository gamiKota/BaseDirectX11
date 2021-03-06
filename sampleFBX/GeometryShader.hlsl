struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 wPos : TEXCOORD1;
};


// ジオメトリシェーダーの入力パラメータ
typedef VS_OUT GS_IN;


struct GS_OUT
{
	float4 pos    : SV_POSITION;
	float2 uv     : TEXCOORD0;
	float3 normal : NORMAL;
	float3 wPos   : TEXCOORD1;
	float4 color  : COLOR0;
};


// 定数バッファ
// cbufferキーワードで宣言
// レジストリはb...バッファを指定
// 構造体のように書くが、構造体ではない。
// メンバ変数をグローバル変数のように扱う。
cbuffer WorldBuffer : register(b3) {
	float4x4 world;
}


[maxvertexcount(3)]   // ジオメトリシェーダーで出力する最大頂点数
// ジオメトリシェーダー
void main(triangle GS_IN In[3],					// トライアングル リストを構成する頂点配列の入力情報
	inout TriangleStream<GS_OUT> TriStream		// 頂点情報を追加するためのストリームオブジェクト
)
{
	GS_OUT Out;

	int i;

	for (i = 0; i < 3; i++)
	{
		Out.pos = In[i].pos;
		Out.uv = In[i].uv;
		Out.normal = In[i].normal;
		Out.wPos = In[i].wPos;
		Out.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}