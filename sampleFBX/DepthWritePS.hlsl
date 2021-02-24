struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 shadowPos : TEXCOORD1;
};

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = float4(1,0,0,1);
	// 座標変換後wにはニアクリップからファークリップまでの距離が格納される。
	// z値をのまま書き込むと1以上の値をとってしまうのが懸念されるので、
	// 全体の長さ(w)で割って0～1の範囲に正規化したデータを書き込む
	color.r = PIN.shadowPos.z / PIN.shadowPos.w;
	return color;
}