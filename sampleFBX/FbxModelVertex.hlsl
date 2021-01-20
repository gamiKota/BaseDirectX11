// FBX�p���_�V�F�[�_ (FbxModelVertex.hlsl)

#define MAX_BONE_MATRIX	256

// �J����
cbuffer global_camera : register(b0) {
	float4	g_cameraPos;	// ���_���W(���[���h���)
	matrix	g_View;			// �r���[
	matrix	g_Proj;			// �v���W�F�N�V����
};

// ���C�g
cbuffer global_light : register(b1) {
	float4	g_lightDir;			// ��������(���[���h���)
	float4	g_lightAmbient;		// ����
	float4	g_lightDiffuse;		// �g�U��
	float4	g_lightSpecular;	// ���ʔ��ˌ�
};

// �`��I�u�W�F�N�g�̍s����
cbuffer global_world : register(b3) {
	matrix	g_World;		// ���[���h�s��
	matrix	g_mTexture;		// �e�N�X�`���s��
};

// �{�[���̃|�[�Y�s��
cbuffer global_bones : register(b5) {
	matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// �p�����[�^
struct VS_INPUT {
	float3	Pos		: POSITION;
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	uint4	Bone	: BONE_INDEX;	// �{�[���̃C���f�b�N�X
	float4	Weight	: BONE_WEIGHT;	// �{�[���̏d��
};

struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;	// wPos
};

// �X�L�j���O��̒��_�E�@��
struct SKIN {
	float4 Pos;
	float3 Norm;
};

// ���_���X�L�j���O (�{�[���ɂ��ړ�)
SKIN SkinVert(VS_INPUT input)
{
	SKIN output;
	float4 Pos = float4(input.Pos, 1.0f);
	float3 Norm = input.Normal;
	if (input.Weight.x == 0.0f) {
		// �{�[������
		output.Pos = Pos;
		output.Norm = Norm;
	}
	else {
		// �{�[��0
		uint uBone = input.Bone.x;
		float fWeight = input.Weight.x;
		matrix m = g_BoneWorld[uBone];
		output.Pos = fWeight * mul(Pos, m);
		output.Norm = fWeight * mul(Norm, (float3x3)m);
		// �{�[��1
		uBone = input.Bone.y;
		fWeight = input.Weight.y;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// �{�[��2
		uBone = input.Bone.z;
		fWeight = input.Weight.z;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// �{�[��3
		uBone = input.Bone.w;
		fWeight = input.Weight.w;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
	}
	return output;
}

//
// ���_�V�F�[�_
//
VS_OUTPUT main(VS_INPUT input)
{
	matrix WVP;
	VS_OUTPUT output;
	SKIN vSkinned	= SkinVert(input);
	WVP				= mul(g_World, g_View);
	WVP				= mul(WVP, g_Proj);
	output.Pos		= mul(vSkinned.Pos, WVP);
	output.Tex		= input.Tex;
	output.Normal	= mul(vSkinned.Norm, (float3x3)g_World);
	output.PosForPS = mul(vSkinned.Pos, g_World).xyz;
	return output;
}
