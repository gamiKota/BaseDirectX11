/**
 * @file VertexShader.hlsl
 */

// �V�F�[�_�ł̓u���[�N�|�C���g�͗��p�ł��Ȃ�
// ��ȃf�o�b�O���@�͊e��p�����[�^��F���Ƃ��āA��ʂɏo�͂��Ă�����
// �������A�o�͂ł���F����0.0�`1.0�͈̔͂ɂȂ�̂ŁA
// 1���傫���f�[�^�͍ő�l�Ŋ���Ȃǂ��āA0.0�`1.0�͈̔͂ɐ��K������K�v������

// �O���[�o��
//cbuffer global : register(b0) {
//	matrix	g_WorldViewProj;	// ���[���h�~�r���[�~�ˉe�s��
//	matrix	g_World;			// ���[���h�s��
//	float4	g_cameraPos;		// ���_���W(���[���h���)
//	float4	g_lightDir;			// ��������(���[���h���)
//	float4	g_lightAmbient;		// ����
//	float4	g_lightDiffuse;		// �g�U��
//	float4	g_lightSpecular;	// ���ʔ��ˌ�
//};

cbuffer WorldBuffer : register(b3) {
	float4x4 world;
}

// �J�����̏��
cbuffer CameraBuffer : register(b4) {
	float4x4 view;
	float4x4 proj;
}

struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn�����
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 wPos : TEXCOORD1;
};



VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);

	// �s��ƃx�N�g���̌v�Z
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.wPos = VOUT.pos.xyz;

	// �J�������W�֕ϊ�
	VOUT.pos = mul(VOUT.pos, view);

	// �J�����̉�p
	VOUT.pos = mul(VOUT.pos, proj);

	// UV���
	VOUT.uv = VIN.uv;

	// �@���̏����s�N�Z���V�F�[�_�ɓn��
	// ����x���[���h���W�ɕϊ�����
	// �@���͈ړ����Ȃ��̂ŁA��]���鐬���݂̂Ōv�Z����
	//VOUT.normal = mul(VIN.normal, (float3x3)world);
	VOUT.normal = VIN.normal;

	return VOUT;

	//VS_OUTPUT VOUT;
	//VOUT.pos = float4(VIN.pos, 1);
	//
	//// �s��ƃx�N�g���̌v�Z
	//VOUT.Pos = mul(VOUT.pos, g_World);
	//
	//VOUT.Pos = mul(VOUT.pos, g_WorldViewProj);
	//
	//VOUT.Tex = input.Tex;
	//VOUT.Normal = mul(vSkinned.Norm, (float3x3)g_World);
	//VOUT.PosForPS = mul(vSkinned.Pos, g_World).xyz;
	//
	//return VOUT;
}

// EOF