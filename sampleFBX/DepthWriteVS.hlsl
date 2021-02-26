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
	// �[�x�l���������ނ��߂ɂ̓s�N�Z���V�F�[�_�ŏ������Ȃ��Ƃ����Ȃ��B
	// �ł��A�[�x�l(���W)�̏��͒��_�V�F�[�_�ł��������ĂȂ��̂ŁA
	// �n����悤�ɂ���
	float4 shadowPos : TEXCOORD1;
};

cbuffer camera : register(b0)
{
	float4	g_cameraPos;	// ���_���W(���[���h���)
	float4x4 view;
	float4x4 proj;
};
cbuffer Light : register(b1) {
	float4	LightDir;			// ��������
	float4	LightAmbient;		// �����F(�A���r�G���g)
	float4	LightDiffuse;		// �����F(�f�B�t���[�Y)
	float4	LightSpecular;		// �����F(�X�y�L����)
	float4x4 LightView;
	float4x4 LightProj;
	float4x4 lightVPS;
};
cbuffer World : register(b3) {
	float4x4 mtxWorld;
	float4x4 mtxTexture;
};
cbuffer LightScreen : register(b7) {

}

VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);
	VOUT.pos = mul(VOUT.pos, mtxWorld);
	VOUT.pos = mul(VOUT.pos, LightView);
	VOUT.pos = mul(VOUT.pos, LightProj);
	// �[�x�l�̓J�������猩�����s���̏��̂��߁A
	// ���[���h���W�ł͂Ȃ��A�r���[���W�𗘗p����
	VOUT.shadowPos = VOUT.pos;

	VOUT.uv = VIN.uv;

	return VOUT;
}