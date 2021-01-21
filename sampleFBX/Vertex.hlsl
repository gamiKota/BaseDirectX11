// ���_�V�F�[�_

//// �O���[�o��
//cbuffer global : register(b0) {
//	matrix g_mWVP;
//	matrix g_mWorld;
//};

// �J����
cbuffer global_camera : register(b0) {
	float4	g_cameraPos;	// ���_���W(���[���h���)
	matrix	g_View;			// �r���[
	matrix	g_Proj;			// �v���W�F�N�V����
};

// �`��I�u�W�F�N�g�̍s����
cbuffer global_world : register(b3) {
	matrix	g_World;		// ���[���h�s��
	matrix	g_mTexture;		// �e�N�X�`���s��
};

// �p�����[�^
struct VS_INPUT {
	float3	Position	: POSITION;
	float3	Normal		: NORMAL;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix WVP;
	float4 P = float4(input.Position, 1.0f);
	WVP = mul(g_World, g_View);
	WVP = mul(WVP, g_Proj);
	output.Position = mul(P, WVP);
	output.Pos4PS = mul(P, g_World).xyz;
	output.Normal = mul(float4(input.Normal, 0.0f), g_World).xyz;
	return output;
}
