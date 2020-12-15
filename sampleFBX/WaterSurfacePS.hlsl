


// �h���C���V�F�[�_�[�̏o�̓p�����[�^
struct DS_OUT
{
	float4 pos   : SV_POSITION;
	float2 texel : TEXCOORD0;

	//float4	Position	: SV_Position;
	//float3	Pos4PS		: TEXCOORD0;
	//float3	Normal		: TEXCOORD1;
	//float2	TexCoord	: TEXCOORD2;
	//float4	Diffuse		: COLOR0;
};


// �e�N�X�`���[
Texture2D g_Tex : register(t0);

// �T���v���[�X�e�[�g
SamplerState  g_Sampler : register(s0);	// �T���v��;


// *****************************************************************************************
// �s�N�Z���V�F�[�_
// *****************************************************************************************
float4 main(DS_OUT In) : SV_TARGET
{
  return g_Tex.Sample(g_Sampler, In.texel);
  //return float4(In.texel, 0.f, 1.f);
}


// EOF