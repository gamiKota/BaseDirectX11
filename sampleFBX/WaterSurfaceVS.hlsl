/**
 * @file WaterSurfaceVS.flsl
 */


// ���_�V�F�[�_�[�̓��̓p�����[�^
struct VS_IN
{
	float3 pos   : POSITION;	// ���_���W
	float2 texel : TEXCOORD;	// �e�N�Z��
};

// ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT
{
	float3 pos    : POSITION;
	float2 texel  : TEXCOORD;
};


VS_OUT main(VS_IN In)
{
	VS_OUT Out;

	// ���_�V�F�[�_�[�ł͂��̂܂ܓn��
	Out.pos = In.pos;
	Out.texel = In.texel;
	return Out;
}

// EOF