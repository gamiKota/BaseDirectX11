// �s�N�Z���V�F�[�_

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

// ���C�g��ON/OFF
cbuffer global_lightsetting : register(b2) {
	float4 g_lightSetting;
};

// �`��I�u�W�F�N�g�̍s����
cbuffer global_world : register(b3) {
	matrix	g_World;		// ���[���h�s��
	matrix	g_mTexture;		// �e�N�X�`���s��
};

// �`��I�u�W�F�N�g�̃}�e���A�����
cbuffer global_material : register(b4) {
	float4	g_Ambient;			// ���F
	float4	g_Diffuse;			// �g�U�F
	float4	g_Specular;			// ���ʔ��ːF
	float4	g_Emissive;			// �����F
};

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
};

float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;

	if (Alpha <= 0.0f) discard;

	if (g_lightDir.x != 0.0f || g_lightDir.y != 0.0f || g_lightDir.z != 0.0f) {
		// �����L��
		float3 L = normalize(-g_lightDir.xyz);				// �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);					// �@���x�N�g��
		float LN = dot(L, N);
		if (LN < 0.0f) {
			LN = -LN;
			N = -N;
		}
		float3 V = normalize(g_cameraPos.xyz - input.Pos4PS);	// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);						// �n�[�t�x�N�g��
		Diff = g_lightAmbient.rgb * g_Ambient.rgb + g_lightDiffuse.rgb *
			Diff * saturate(LN);						// �g�U�F + ���F
		float3 Spec = g_lightSpecular.rgb * g_Specular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a);				// ���ʔ��ːF
		Diff += Spec;
	}

	Diff += g_Emissive.rgb;										// �����F

	return float4(Diff, Alpha);
}
