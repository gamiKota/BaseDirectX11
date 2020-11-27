struct VS_OUT
{
	float4	Position	: SV_POSITION;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};


// ジオメトリシェーダーの入力パラメータ
typedef VS_OUT GS_IN;


struct GS_OUT
{
	float4	Position	: SV_POSITION;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};


cbuffer WorldBuffer : register(b0) {
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
		Out.Position = In[i].Position;
		Out.TexCoord = In[i].TexCoord;
		Out.Diffuse = In[i].Diffuse;

		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}