// FBX�p�s�N�Z���V�F�[�_ (FbxModelPixel.hlsl)

// �p�����[�^
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	return float4(0.f, 0.f, 0.f, 1.f);
}
